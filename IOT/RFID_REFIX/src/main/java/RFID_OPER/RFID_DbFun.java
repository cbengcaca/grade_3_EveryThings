package RFID_OPER;

import java.sql.*;

import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

public class RFID_DbFun {
    private Connection conn = null;
    private Statement stmt = null;
    private String camLocate = null;

    public String getCamLocate(){
        return camLocate;
    }

    public void connect() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/iot?useSSL=false&serverTimezone=UTC&allowPublicKeyRetrieval=true", "root", "19990418");
            stmt = conn.createStatement();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            System.out.println("SQL ERROR");
            e.printStackTrace();
        }
    }

    public void disConnect() {
        try {
            conn.close();
            stmt.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * 根据输入相片路径，金额，卡号，进行数据库操作
     * @param cardId
     * @param charge
     * @return 操作是否成功
     * @throws SQLException
     */
    public String cardIdAndBalance(String cardId, float charge) throws SQLException {
        connect();
        String flag = "success";
        float result = 0;
        String studentId = null;
        int recordMax = 0;

        ResultSet rs = stmt.executeQuery("select id from cardid where cardid = '" + cardId + "'");
        if(rs.next()){
            //卡号存在
            camLocate = "iotCamera" + "/" + System.currentTimeMillis() + ".png";
            studentId = rs.getString("id");
            rs = stmt.executeQuery("select balance from idbalance where id = '" + studentId +"'");
            rs.next();
            result = rs.getFloat("balance");

            //查找下一序号
            rs = stmt.executeQuery("select max(balogid) from balancelog");
            rs.next();
            recordMax = rs.getInt(1);
            recordMax++;

            if (result >= charge) {
                //余额足，则进行更新
                result -= charge;
                stmt.executeUpdate("update idbalance set balance = " + result + " where id = '" + studentId + "'");
                stmt.executeUpdate("insert into balancelog values(" + recordMax + ",'" + studentId + "'," + charge +",now(),'success')");
            }else{
                //余额不足
                flag = "lowcharge";
                stmt.executeUpdate("insert into balancelog values(" + recordMax + ",'" + studentId + "'," + charge +",now(),'failed')");
            }
            stmt.executeUpdate("insert into balogcam values('" + recordMax + "','" + camLocate + "')");
            disConnect();
        }else{
            //卡号不存在
            flag = "cardnotfound";
        }
        return flag;
    }

    /**
     * 充值
     * @param cardId
     * @param charge
     * @return
     * @throws SQLException
     */
    public String reCharge(String cardId,float charge) throws SQLException {
        connect();

        String result = "success";
        String studentId = null;
        float balance = 0;
        int recordMax = 0;

        ResultSet rs = stmt.executeQuery("select id from cardid where cardid = '" + cardId + "'");
        if(rs.next()) {
            //卡号存在
            studentId = rs.getString("id");
            rs = stmt.executeQuery("select balance from idbalance where id = '" + studentId +"'");
            rs.next();
            balance = rs.getFloat("balance");
            //查找下一序号
            rs = stmt.executeQuery("select max(bulogid) from businesslog");
            rs.next();
            recordMax = rs.getInt(1);
            recordMax++;

            balance += charge;
            //增加余额
            stmt.executeUpdate("update idbalance set balance = " + balance + " where id = '" + studentId + "'");
            stmt.executeUpdate("insert into businesslog values(" + recordMax + ",'" + studentId + "',now(),'recharge')");
        }else{
            //卡号不存在
            result = "cardnotfound";
        }

        disConnect();
        return result;
    }
}

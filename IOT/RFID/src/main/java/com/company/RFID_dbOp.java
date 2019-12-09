package com.company;

import java.sql.*;
import java.util.ArrayList;


public class RFID_dbOp {
    private Connection conn = null;
    private Statement stmt = null;


    /**
     * db连接
     */
    public void RFID_connectDB(){
        try {
            Class.forName("com.mysql.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/RFID?useSSL=false&serverTimezone=UTC", "root", "19990418");
            stmt = conn.createStatement();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }catch (SQLException e){
            System.out.println("SQL ERROR" );
            e.printStackTrace();
        }
    }

    /**
     * db断开连接
     */
    public void RFID_disConnectDB(){
        try {
            conn.close();
            stmt.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }


    /**
     * 根据卡号找学号
     * @param cardId 卡号
     * @return 学号
     */
    public String RFID_dbFindId(String cardId){
        RFID_connectDB();
        String sql = "select id from cardid where cardid = '"+ cardId + "'";
        String result = new String();
        try {
            ResultSet rs  = stmt.executeQuery(sql);
            while (rs.next()){
                result = rs.getString("id");
            }
            rs.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        RFID_disConnectDB();
        return result;
    }

    /**
     * 利用学号查找余额
     * @param id
     * @return 余额
     */
    public float RFID_dbFindBalance(String id){
        RFID_connectDB();
        String sql = "select balance from idbalance where id = '"+ id + "'";
        float balance = -1;
        try {
            ResultSet rs  = stmt.executeQuery(sql);
            while (rs.next()){
                balance = rs.getFloat("balance");
            }
            rs.close();
        }catch (SQLException e){
            e.printStackTrace();
        }
        RFID_disConnectDB();
        return balance;
    }

    public int RFID_dbFindBalogidMax(){
        RFID_connectDB();
        int max = 0;

        boolean empty = true;
        try {
            ResultSet rs  = stmt.executeQuery("select * from balancelog");
            while (rs.next()){
                empty = false;
            }
        }catch (SQLException e){
            e.printStackTrace();
        }
        if(empty == true){
            return 0;
        }else{
            try {
                ResultSet rs  = stmt.executeQuery("select max(balogid) from balancelog");
                while (rs.next()){
                    max = rs.getInt("balogid");
                }
            }catch (SQLException e){
                e.printStackTrace();
            }
        }

        RFID_disConnectDB();
        return max;
    }

    /**
     * 非查询的dbop操作
     * @param sql 外部传入sql语句
     */
    public void RFID_dbUpdate(String sql){
        RFID_connectDB();
        try {
            stmt.executeUpdate(sql);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        RFID_disConnectDB();
    }

    /*
    public static void main(String []args){
        RFID_dbOp rfid_dbOp = new RFID_dbOp();
        String sql = " select * from nameid; ";
        rfid_dbOp.RFID_dbSearch(sql);
    }*/
}

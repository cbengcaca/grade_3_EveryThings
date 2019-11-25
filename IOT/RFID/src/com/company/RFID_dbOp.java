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
     * 查询学生姓名与id
     * @param sql
     * @return
     */
    public ArrayList<String[]> RFID_dbSearch(String sql){
        RFID_connectDB();

        ArrayList<String[]> searchResult = new ArrayList<String[]>();

        try {
            ResultSet rs  = stmt.executeQuery(sql);
            while (rs.next()){
                String []line = new String[2];
                line[0] = rs.getString("id");
                line[1] = rs.getString("name");
                searchResult.add(line);
            }
            rs.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }


        RFID_disConnectDB();
        return searchResult;
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

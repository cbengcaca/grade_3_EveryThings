package com.company;

public class RFID_actions {
    private RFID_atomicOp rfid_atomicOp;
    private RFID_dbOp rfid_dbOp;
    private RFID_cam rfid_cam;

    public RFID_actions(){
        rfid_atomicOp = new RFID_atomicOp();
        rfid_dbOp = new RFID_dbOp();
        rfid_cam = new RFID_cam();

    }

    public boolean ACTION_pay(float money){
        boolean successFlag = false;
        rfid_atomicOp.RFID_read();
        //执行成功
        if(rfid_atomicOp.getLatestOp() == true){
            //读取卡id
            long [] long_cardId = rfid_atomicOp.getCardID();
            //转换为字符串
            String str_cardId = new String();
            for (long temp:long_cardId
            ) {
                str_cardId += Long.toString(temp);
            }
            //照相，获取相片路径
            rfid_cam.takephoto();
            String camLocate = rfid_cam.getLocate();

            //写数据库
            //余额-
            //消费记录+
            //相片记录+
            //查看余额是否足够

            String id = rfid_dbOp.RFID_dbFindId(str_cardId);
            float balance = rfid_dbOp.RFID_dbFindBalance(id);
            balance -= money;

            //查找最大标号
            int maxBalogId = rfid_dbOp.RFID_dbFindBalogidMax();
            //余额充足，更新表信息
            if(balance >= 0){
                rfid_dbOp.RFID_dbUpdate("update idbalance set balance = '" + balance + "' where id = '" + id + "'");
                rfid_dbOp.RFID_dbUpdate("insert into balancelog values('" + maxBalogId + "','" + id + "','" + money +"','now()','success'");
                successFlag = true;
            }
            //余额不足，更新表信息
            else{
                rfid_dbOp.RFID_dbUpdate("insert into balancelog values('" + maxBalogId + "','" + id + "','" + money +"','now()','failed'");
            }
            rfid_dbOp.RFID_dbUpdate("insert into balogcam values('" + maxBalogId + "','" + camLocate + "'");

        }

        return successFlag;
    }

    public boolean ACTION_recharge(float money){

        rfid_atomicOp.RFID_read();
        //执行成功
        if(rfid_atomicOp.getLatestOp() == true) {
            //读取卡id
            long[] long_cardId = rfid_atomicOp.getCardID();
            //转换为字符串
            String str_cardId = new String();
            for (long temp : long_cardId
            ) {
                str_cardId += Long.toString(temp);
            }
            String id = rfid_dbOp.RFID_dbFindId(str_cardId);
            float balance = rfid_dbOp.RFID_dbFindBalance(id);
            balance += money;
            rfid_dbOp.RFID_dbUpdate("update idbalance set balance = '" + balance + "' where id = '" + id + "'");

        }

        return true;
    }
}

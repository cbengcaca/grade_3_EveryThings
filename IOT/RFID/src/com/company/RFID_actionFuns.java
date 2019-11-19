package com.company;

public class RFID_actionFuns {
    private Main RFID_handler;

    public RFID_actionFuns(){
        RFID_handler = new Main();
    }

    /**
     * 根据学号进行注册
     * @param studentID 学号
     */
    public void RFIDaction_Regist(String studentID){

    }

    /**
     * 根据学号与金额进行充值
     * @param studentID 学号
     * @param money 金额
     */
    public void RFIDaction_Recharge(String studentID,float money){

    }

    /**
     * 根据学号金额进行开销
     * @param studentID 学号
     * @param money 金额
     */
    public void RFIDaction_Cost(String studentID,float money){

    }
}

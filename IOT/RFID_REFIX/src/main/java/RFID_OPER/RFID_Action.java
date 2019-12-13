package RFID_OPER;

import java.sql.SQLException;

public class RFID_Action {
    private RFID_ReaderFun rfid_readerFun;
    private RFID_DbFun rfid_dbFun;
    private RFID_CAM rfid_cam;

    public RFID_Action(){
        rfid_readerFun = new RFID_ReaderFun();
        rfid_cam = new RFID_CAM();
        rfid_dbFun = new RFID_DbFun();
    }

    /**
     * 付款
     * @param charge
     * @return
     */
    //边界情况有3种
    //1读卡超时
    //2卡号不正确
    //3余额不足
    public String pay(float charge){
        String result = "success";
        //刷卡
        String cardId = rfid_readerFun.readCard();

        if(!cardId.equals("timeout")){
            //读卡成功

            try {
                //判断卡号以及余额是否充足
                String avili = rfid_dbFun.cardIdAndBalance(cardId,charge);

                if(!avili.equals("cardnotfound")){
                    //卡号存在，照相
                    //照相
                    String locate = rfid_dbFun.getCamLocate();
                    rfid_cam.takephoto(locate);
                }
                result = avili;
            } catch (SQLException e) {
                e.printStackTrace();
            }

        }else{
            //读卡超时
            result = "timeout";
        }
        return result;
    }

    /**
     * 充值
     * @param charge
     * @return 结果
     */
    //边界情况有2种
    //1读卡超时
    //2卡片错误
    public String reCharge(float charge){
        String result = "success";
        //刷卡
        String cardId = rfid_readerFun.readCard();
        if(!cardId.equals("timeout")){
            //读卡成功
            try {
                result = rfid_dbFun.reCharge(cardId,charge);

            } catch (SQLException e) {
                e.printStackTrace();
            }
        }else{
            //读卡超时
            result = "timeout";
        }
        return result;
    }
}

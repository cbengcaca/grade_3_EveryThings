package RFID_OPER;
import jAB.ab_javacall;

public class RFID_ReaderFun {

    /**
     * 单次开启串口进行一次读操作，之后关闭
     * @return 卡号
     */
    public String readCard() {
        char []cardId = new char[10];
        ReadCardThread readCardThread = new ReadCardThread(cardId);
        readCardThread.start();
        try {
            readCardThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return readCardThread.getCardId();
    }
}

class ReadCardThread extends Thread {
    private char[] cardId;
    public ReadCardThread(char[] cardId){
        this.cardId = cardId;
    }
    @Override
    public void run() {
        cardId = new char[10];
        cardId[0] = '0';
        int handler = ab_javacall.lc_init_ex(1,"COM3".toCharArray(),9600);
        if(handler != -1){
            System.out.println("OPEN SERIAL SUCCESS");
        }else{
            System.out.println("OPEN SERIAL FAILED");
        }
        try {
            Thread.sleep(170);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        boolean timeOutFlag = true;
        for (int i=0;i<50;i++){
            ab_javacall.lc_card_str(handler,(byte)0,cardId);
            //规定时间内读到卡
            if(cardId[0] != '0'){
                ab_javacall.lc_beep(handler,1);
                ab_javacall.lc_beep(handler,1);
                timeOutFlag = false;
                break;
            }
        }
        //超时
        if(timeOutFlag){
            cardId = "timeout".toCharArray();
            ab_javacall.lc_beep(handler,1);
            ab_javacall.lc_beep(handler,1);
            ab_javacall.lc_beep(handler,1);
            ab_javacall.lc_beep(handler,1);
            ab_javacall.lc_beep(handler,1);
            ab_javacall.lc_beep(handler,1);
        }
        ab_javacall.lc_exit(handler);
    }

    public String getCardId(){
        return String.valueOf(cardId);
    }

}
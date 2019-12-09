package com.company;

import jAB.ab_javacall;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import java.io.File;
import java.io.IOException;

public class RFID_atomicOp {
    private int handler;
    private String comName;
    private int port;
    private long[] cardID;
    private boolean latestOpFlag;
    private boolean timeOutFlag;

    RFID_atomicOp(){
        //初始化xml解析类
        SAXReader reader = new SAXReader();
        Document doc = null;
        try {
            doc = reader.read(new File("src\\main\\java\\staticData\\RFID_info"));
        } catch (DocumentException e) {
            e.printStackTrace();
        }
        //获得根节点
        Element root = ((org.dom4j.Document) doc).getRootElement();
        //获得comName标签
        Element comName = root.element("comName");
        this.comName = comName.getText();
        Element port = root.element("port");
        this.port = Integer.parseInt(port.getText());
    }

    public void RFID_read(){
        int status;
        cardID = new long[20];
        latestOpFlag = false;
        timeOutFlag = false;

        status = ab_javacall.lc_init_ex(1, comName.toCharArray(),115200);
        handler = status;
        if(handler == -1){
            System.out.println("Open failed");
            latestOpFlag = false;
            return;
        }else{
            System.out.println("Open success");
        }

        status = -1;
        status = ab_javacall.lc_setReaderMode(handler, (byte)0);
        if(status == 0){
            System.out.println("Modeset success");
        }else{
            System.out.println("Modeset failed");
            latestOpFlag = false;
            return;
        }
        status = -1;


        //读卡延时
        for (int i=0; i<30;i++){
            status = ab_javacall.lc_card(handler,(byte)0,cardID);
            if(cardID[0] != 0x00){
                RFID_BBBBB(handler,1,2);
                timeOutFlag = true;
                break;
            }
        }
        if(status == 0){
            System.out.println("Card find success");
            latestOpFlag = true;
        }else{
            System.out.println("Card find failed");
            latestOpFlag = false;
        }

        //如果读取时间超时，进行蜂鸣
        if(timeOutFlag){
            RFID_BBBBB(handler,1,6);
        }
        ab_javacall.lc_exit(handler);
    }

    /**
     * 蜂鸣器
     * @param deviceFlag 从open获得的串口标识符
     * @param bbTime 单次蜂鸣时间
     * @param times 总共蜂鸣次数
     */
    public void RFID_BBBBB(int deviceFlag,int bbTime, int times){
        for (int i=0;i<times;i++){
            ab_javacall.lc_beep(deviceFlag, bbTime);
        }
    }

    public long[] getCardID(){
        return cardID;
    }

    public boolean getLatestOp(){
        return latestOpFlag;
    }

    public boolean getTimeOut(){
        return timeOutFlag;
    }


}

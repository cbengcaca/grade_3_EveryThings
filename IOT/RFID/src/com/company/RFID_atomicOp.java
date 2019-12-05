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
            doc = reader.read(new File("src\\staticData\\RFID_info"));
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

        cardID = new long[20];
        latestOpFlag = false;
        timeOutFlag = false;
    }

    //开启串口
    //读写模式设定
    //寻卡
    //验证
    //读卡

    public void RFID_read(){
        int status;

        /**
         * 串口开启操作
         * @return 分配成功的串口标识符：-1-失败||<>-1-成功
         */
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
        /**
         * 读卡器运行模式选择
         * @param modeSelect 模式选择：0，被动读卡||1，主动读卡，10进制输出||2，主动读卡，16进制输出
         * @return 操作是否成功 0-成功||<>0-失败
         */
        status = ab_javacall.lc_setReaderMode(handler, (byte)0);
        if(status == 0){
            System.out.println("Modeset success");
        }else{
            System.out.println("Modeset failed");
            latestOpFlag = false;
            return;
        }
        status = -1;

        /**
         * 寻卡操作
         * @param deviceFlag 从open中获取的设备标识符
         * @param selectMode 寻卡模式：0-单卡模式||1-多卡模式
         * @param cardID 读出的ic卡卡号
         * @return 操作是否成功：0成功||非0失败
         */
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

    public static void main(String[] args) throws IOException {
        RFID_atomicOp temp = new RFID_atomicOp();
        temp.RFID_read();

    }
}

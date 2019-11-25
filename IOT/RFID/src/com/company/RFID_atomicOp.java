package com.company;

import jAB.ab_javacall;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class RFID_atomicOp {

    /**
     * 串口开启操作
     * @param comName 串口名称
     * @return 分配成功的串口标识符：-1-失败||<>-1-成功
     */
    public int RFID_open(String comName){
        int status;
        status = ab_javacall.lc_init_ex(1, comName.toCharArray(),115200);
        return status;
    }

    /**
     * 读卡器运行模式选择
     * @param deviceFlag 从open中获取的设备标识符
     * @param modeSelect 模式选择：0，被动读卡||1，主动读卡，10进制输出||2，主动读卡，16进制输出
     * @return 操作是否成功 0-成功||<>0-失败
     */
    public int RFID_modeSet(int deviceFlag,byte modeSelect){
        int status;
        status = ab_javacall.lc_setReaderMode(deviceFlag, modeSelect);
        return status;
    }

    /**
     * 寻卡操作
     * @param deviceFlag 从open中获取的设备标识符
     * @param selectMode 寻卡模式：0-单卡模式||1-多卡模式
     * @param cardID 读出的ic卡卡号
     * @return 操作是否成功：0成功||非0失败
     */
    public int RFID_findCard(int deviceFlag,byte selectMode,long[]cardID){
        int status;
        status = ab_javacall.lc_card(deviceFlag,selectMode,cardID);
        return status;
    }

    /**
     * 验证操作
     * @param deviceFlag 从open获取的串口标识符
     * @param selectMode 密码模式选择：0x60-密码A||0x61-密码B
     * @param selectBlock 选择验证的块号：0-63号
     * @param key 区域密码
     * @return 操作是否成功：0成功||非0失败
     */
    public int RFID_authenticate(int deviceFlag,byte selectMode,int selectBlock,byte[]key){
        int status;
        status = ab_javacall.lc_authenticate(deviceFlag, selectMode, (byte)1, key);
        return status;
    }

    /**
     * 写卡操作
     * @param deviceFlag 从open获得的串口标识符
     * @param selectBlock 选择写的选区：0-63
     * @param writeMessage 写入的数据
     * @return 0成功||非0失败
     */
    public int RFID_write(int deviceFlag,byte selectBlock,byte []writeMessage){
        int status;
        status = ab_javacall.lc_mfWrite(deviceFlag,selectBlock, writeMessage);
        return status;
    }

    /**
     * 读卡操作
     * @param deviceFlag 从open获得的串口标识符
     * @param selectBlock 读选区：0-63
     * @param readMessage 读出信息存储
     * @return 0失败||非0成功
     */
    public int RFID_read(int deviceFlag,byte selectBlock,byte []readMessage){
        int status;
        status = ab_javacall.lc_mfRead(deviceFlag, selectBlock, readMessage );
        return status;
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

    /**
     * 关闭串口
     * @param deviceFlag 从open获得串口标识符
     */
    public void RFID_close(int deviceFlag){
        ab_javacall.lc_exit(deviceFlag);
    }

    /*public static void main(String[] args) throws IOException {
        /*
        Main handler = new Main();
        String comName = "COM1";
        int handleFlag;
        handleFlag = handler.RFID_open(comName);
        if(handleFlag == -1){
            System.out.println("open failed");
        }else{
            System.out.println("open success");
        }

        byte mode = 0;
        handler.RFID_modeSet(handleFlag,mode);


        long[] cardID = new long[20];
        handler.RFID_findCard(handleFlag,(byte)0,cardID);



        byte selectBlock = 0;
        byte []message = new byte[20];

        handler.RFID_read(handleFlag,selectBlock,message);

        for (byte a:message) {
            System.out.print(a);
        }

        handler.RFID_close(handleFlag);
        /*
        int port = 55533;
        ServerSocket server = new ServerSocket(port);
        Socket socket = server.accept();

        Main handler = new Main();
        int status;

        //定义开启串口名
        String comName = "COM1";
        int deviceFlag = handler.RFID_open(comName);
        if (deviceFlag != -1) {
            System.out.println("RFID open success");
        }else{
            System.out.println("RFID open failed");
            return;
        }

        //读卡器运行模式
        byte runningMode = 0;
        status = handler.RFID_modeSet(deviceFlag,runningMode);
        if(status == 0){
            System.out.println("Running mode set success\n");
        }

        //寻卡
        byte findMode = 0;
        char[] cardID = new char[20];
        status = handler.RFID_findCard(deviceFlag,findMode,cardID);
        if(status == 0){
            System.out.println("Card find success\n");
            System.out.print("CARD ID = ");
            for (char num:cardID) {
                System.out.print(num);
            }
            System.out.println();
        }else{
            System.out.println("Card find failed");
        }

        //验证卡片
        byte []defKey = {(byte)0xff, (byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff,(byte)0xff};
        byte selectMode = 0x60;
        int selectBlock = 1;
        status = handler.RFID_authenticate(deviceFlag,selectMode,selectBlock,defKey);



    }*/
}

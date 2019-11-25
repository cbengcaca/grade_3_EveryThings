package com.company;


import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import org.dom4j.*;
import org.dom4j.io.*;

public class RFID_actionFuns {
    private RFID_atomicOp RFID_handler;
    private String comName;
    private RFID_dbOp RFID_dboper;

    private ServerSocket server;
    private Socket socket;
    private int port;

    public void setServerSocket(ServerSocket server){
        this.server = server;
    }
    public ServerSocket getServerSocket(){
        return this.server;
    }
    public void setSocket(Socket socket){
        this.socket = socket;
    }
    public Socket getSocket(){
        return this.socket;
    }

    public RFID_actionFuns(){
        //初始化硬件操作类
        RFID_handler = new RFID_atomicOp();
        RFID_dboper = new RFID_dbOp();

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

    }

    /**
     * 根据学号进行注册
     */
    public void RFIDaction_Regist(){
        int handler = RFID_handler.RFID_open(comName);
        //获得全部未办理学生卡的学号名单
        //循环处理学号
            //寻卡号
            //写入卡号学号表的映射关系
            //写入学号余额表，余额为0
            //写入业务交易记录表，类型为新办理，date为当前时间
        //socket返回本次办理的名单
        String sql = "select "
        RFID_handler.RFID_close(handler);

    }

    /**
     * 根据学号进行补办
     */
    public void RFIDaction_issue(){
        int handler = RFID_handler.RFID_open(comName);
        //socket获取学号
        //rfid读取新卡号
        //更新卡号学号表，修改卡号为新卡号
        //写入业务交易记录表，类型为补卡
        //socket返回成功信息
        RFID_handler.RFID_close(handler);
    }


    /**
     * 根据卡号与金额进行充值
     */
    public void RFIDaction_Recharge(){
        int handler = RFID_handler.RFID_open(comName);
        //rfid读卡号
        //db搜索余额
        //socket发回手机
        //host等待
        //接收充值金额
        //更新表学号余额表+=charge
        //socket关闭
        RFID_handler.RFID_close(handler);
    }

    /**
     * 根据学号金额进行开销
     */
    public void RFIDaction_Cost(){
        int handler = RFID_handler.RFID_open(comName);
        //host等待
        //socket接收金额
        //rfid读取卡号
        //db获取余额
        //余额充足
            //更新学号余额表-=cost
            //返回成功信息
        //余额不足
            //返回失败信息
        //返回当前余额
        //socket关闭
        try {
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        RFID_handler.RFID_close(handler);
    }

    public void RFID_WAITING(){
        while(true) {
            try {
                this.server = new ServerSocket(this.port);
                this.socket = server.accept();
                System.out.println("远程主机地址：" + socket.getRemoteSocketAddress());
                DataInputStream in = new DataInputStream(socket.getInputStream());

                String messageIn = in.readUTF();
                //注册
                if (messageIn == "11") {
                    RFIDaction_Regist();
                }
                //补办
                else if (messageIn == "12") {
                    RFIDaction_issue();
                }
                //充值
                else if (messageIn == "13") {
                    RFIDaction_Recharge();
                }
                //开销
                else if (messageIn == "14") {
                    RFIDaction_Cost();
                }
                //系统关闭
                else if (messageIn == "0") {
                    System.out.println("System terminal");
                    socket.close();
                    server.close();
                    break;
                } else {
                    System.out.println("Access Denied");
                    continue;
                }
                //DataOutputStream out = new DataOutputStream(socket.getOutputStream());
                //out.writeUTF("谢谢连接我：" + server.getLocalSocketAddress() + "\nGoodbye!");
                //socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }

    public static void main(String []args) throws DocumentException {
        RFID_actionFuns temp = new RFID_actionFuns();
        temp.RFID_WAITING();
    }
}

package GUI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.DecimalFormat;
import com.company.*;
import com.sun.org.apache.bcel.internal.generic.FLOAD;

public class RFID_GUI extends JFrame{
    private JPanel panel_main = new JPanel();
    private JFrame frame = new JFrame();
    private JPanel panel_text = new JPanel();
    private JPanel panel_btn = new JPanel();
    private JTextField tf_key = new JTextField(15);
    private JTextField tf_money = new JTextField(15);
    //key存储当前目标消费金额
    //money存储读入卡的余额
    private String key = "";
    private String money = "";
    private boolean isRead = false;
    private  boolean isWrite = false;
    DecimalFormat df = new DecimalFormat("#.00");
    RFID_actions rfid_actions;

    public void setMoney(double n){
        money += n;
        System.out.println(money);
    }
//    public void readCard(double k){
//        System.out.println(k);
//        df.format(k);
//        isRead = true;
//        setMoney(k);
//    }

//    public boolean writeMoey(){
//        System.out.println("充值后，金额为" +money);
//        return true;
//    }

    public void setJF(boolean bool){
        panel_btn.removeAll();
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frame.setResizable(false);
        //设置窗体大小以及窗体出现位置
        frame.setSize(600,400);
        frame.setLocation(200,200);

        panel_text.setLayout(new GridLayout(2,1));
        tf_key.setText("--请输入金额--");
        if(!isRead){
            tf_money.setText("未读入余额！！");
        }else {
            tf_money.setText(money);
        }
        panel_text.add(tf_money);
        panel_text.add(tf_key);
        frame.getContentPane().add(panel_text,BorderLayout.NORTH);
        frame.getContentPane().add(panel_btn,BorderLayout.CENTER);
        if(bool){
            frame.setTitle("刷卡机");
            panel_btn.setLayout(new GridLayout(4, 4, 3, 3));
            String str[] = {"7", "8", "9", "清除", "4", "5", "6", "取消", "1", "2", "3", "确认", "0", ".", "回退", "B"};
            JButton btn[] = new JButton[str.length];
            Font font = new Font("仿宋", Font.BOLD, 25);
            for (int i = 0; i < str.length; i++) {
                btn[i] = new JButton(str[i]);
                btn[i].setActionCommand(str[i]);
                btn[i].setFont(font);
                panel_btn.add(btn[i]);
                btn[i].addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        if (e.getActionCommand().equals("1")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "1";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("2")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "2";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("3")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "3";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("4")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "4";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("5")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "5";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("6")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "6";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("7")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "7";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("8")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "8";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("9")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "9";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals("0")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "";
                            }
                            key += "0";
                            tf_key.setText(key);
                        } else if (e.getActionCommand().equals(".")) {
                            if(isWrite == false){
                                isWrite =true;
                                key = "0";
                            }
                            if(key.contains(".")){
                            }else{
                                key += ".";
                                tf_key.setText(key);
                            }
                        }else if(e.getActionCommand().equals("清除")){
                            key = "";
                            tf_key.setText(key);

                        }else if(e.getActionCommand().equals("取消")){
                            key = "--请输入金额--";
                            tf_key.setText(key);
                            isWrite = false;


                        }else if(e.getActionCommand().equals("确认")){
                            if(isWrite){
                                
                                boolean success = rfid_actions.ACTION_pay(Float.parseFloat(key));
                                key = "--请输入金额--";
                                tf_key.setText(key);
                                isWrite = false;
                                if (success){
                                    System.out.println("PAY SUCCESS!");
                                }else{
                                    System.out.println("ERROR!PAY FAILED!");
                                }
                            }else{
                                System.out.println("HAS NO KEY!");
                            }

//                            if(isRead == true && key != ""){
//                                tf_key.setText(key);
//                                double temp = Double.valueOf(money) -Double.valueOf(key);
//                                if(temp >= 0){
//                                    df.format(temp);
//                                    key = "";
//                                    money = "" + temp;
//                                    tf_money.setText(money);
//                                    key = "--请输入金额--";
//                                    tf_key.setText(key);
//                                }else {
//                                    System.out.println("余额不足");
//                                }
//                                isWrite = false;
//                            }else{
//                                System.out.println("ERROR!BO NOT READING CARD!");
//                                key = "--请输入金额--";
//                                tf_key.setText(key);
//                                isWrite = false;
//                            }
                        }else if(e.getActionCommand().equals("回退")){
                            key = key.substring(0, key.length() -1);
                            tf_key.setText(key);
                        }
                        System.out.println("key:" + key + ",money:" + money);
                    }
                });
            }
        }else {
            panel_btn.removeAll();
            frame.setTitle("圈存机");
            JButton btn_confirm = new JButton("充值");
            panel_btn.add(btn_confirm);
            final JLabel l = new JLabel("欢迎使用圈存机");
            panel_btn.add(l);
            btn_confirm.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent actionEvent) {
                    key = tf_key.getText();
                    if(key != "" && Double.valueOf(key) >= 0){

                        boolean success = rfid_actions.ACTION_recharge(Float.parseFloat(key));
                        //df.format();
                        tf_key.setText(key);
                        tf_money.setText(money);
                        l.setText("充值成功！！");
                    }
                }
            });
        }

    }

    public RFID_GUI(){
        //readCard(165.34);

        panel_main.setLayout(new GridLayout(2,2));
        JButton btn_POS = new JButton("刷卡机");
        btn_POS.setSize(50,20);
        JButton btn_transferMachine = new JButton("圈存机");
        btn_transferMachine.setSize(50,20);
        panel_main.add(new JLabel("欢迎使用RFID阅读器"));
        panel_main.add(new JLabel(""));
        panel_main.add(btn_POS);
        panel_main.add(btn_transferMachine);
        add(panel_main);
        btn_POS.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                setJF(true);
            }
        });
        btn_transferMachine.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent actionEvent) {
                setJF(false);
            }
        });

        rfid_actions = new RFID_actions();
    }


    public static void main(String[] args){
        RFID_GUI window=new RFID_GUI();        //设置Jframe窗口的基本配置
        window.setTitle("RFID卡阅读器");
        window.setBounds(300,300,300,100);
        window.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);//添加最大化最小化组件
        window.setVisible(true);    //显示窗口
    }
}

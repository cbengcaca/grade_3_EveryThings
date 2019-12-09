package com.company;

import com.github.sarxos.webcam.Webcam;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import javax.imageio.ImageIO;


public class RFID_cam {
    private Webcam webcam;
    private String locate;

    public String getLocate(){
        return locate;
    }

    public RFID_cam() {
        webcam = Webcam.getDefault();
        webcam.setViewSize(new Dimension(640, 480));
        webcam.open();
        locate = new String();
    }

    public void takephoto() {
        try {
            BufferedImage image = webcam.getImage();
            String pacakgeName = "iotCamera";
            File file = new File(pacakgeName);
            if (!file.exists() && !file.isDirectory()) {
                file.mkdir();
            }

            locate = pacakgeName + "/" + System.currentTimeMillis();
            locate += ".png";
            ImageIO.write(image, "PNG", new File(locate));
        } catch (IOException e) {
            System.out.println("Take photo failed");
            e.printStackTrace();
        }

    }
}

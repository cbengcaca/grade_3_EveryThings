package RFID_OPER;

import com.github.sarxos.webcam.Webcam;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import javax.imageio.ImageIO;

/**
 * 构造时便打开照相接口就绪
 */
public class RFID_CAM {
    private Webcam webcam;

    public RFID_CAM() {
        webcam = Webcam.getDefault();
        webcam.setViewSize(new Dimension(640, 480));
        webcam.open();
    }

    public String takephoto(String locate) {
        try {
            BufferedImage image = webcam.getImage();
            String pacakgeName = "iotCamera";
            File file = new File(pacakgeName);
            if (!file.exists() && !file.isDirectory()) {
                file.mkdir();
            }

            ImageIO.write(image, "PNG", new File(locate));
        } catch (IOException e) {
            System.out.println("Take photo failed");
            e.printStackTrace();
        }
        return locate;
    }
}

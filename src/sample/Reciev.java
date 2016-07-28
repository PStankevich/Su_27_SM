package sample;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Arrays;

public class Reciev {
    private static SerialPort serialPort;
    private static String msg;
    private static int count;
    private static int countNum;



    public static void main(String args[]) {
        try {
            int port = 20083;



            serialPort = new SerialPort("COM13");
            serialPort.openPort();
            serialPort.setParams(SerialPort.BAUDRATE_19200,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);
            serialPort.addEventListener(new Reciev.PortReader(), SerialPort.MASK_RXCHAR);
            DatagramSocket dsocket = new DatagramSocket(port);
            byte[] buffer = new byte[2048];
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

//            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
//            serialPort.writeString(String.valueOf(reader.readLine()));
            while(true){
                dsocket.receive(packet);
                msg = new String(buffer, 0, packet.getLength());
//                System.out.println(packet.getAddress().getHostName() + ": " + buffer.length + " === "
//                        + msg);

                double[] numArr = Arrays.stream(msg.split(" ")).mapToDouble(Double::parseDouble).toArray();
                System.out.println(numArr.length + " " + msg + '/');
                serialPort.writeString(numArr.length + " " + msg + '/');
                packet.setLength(buffer.length);
            }
        } catch (Exception e) {
            System.err.println(e);
        }
        finally {

        }
    }




    private static class PortReader implements SerialPortEventListener {

        public void serialEvent(SerialPortEvent event) {
            if(event.isRXCHAR() && event.getEventValue() > 0){
                try {
                    String data = serialPort.readString(event.getEventValue());
                    System.out.println();
                    System.out.println(data);
                }
                catch (SerialPortException ex) {
                    System.out.println(ex);
                }
            }
        }
    }
}

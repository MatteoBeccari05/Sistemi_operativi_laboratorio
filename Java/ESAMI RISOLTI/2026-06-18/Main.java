import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class Main {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;

        try {
            pos = new PipedOutputStream(pis);
        } catch (Exception e) {
            e.printStackTrace();
        }

        SimulaVento sv = new SimulaVento(pos);
        MonitoraTurbina mt = new MonitoraTurbina(pis);

        sv.start();
        mt.start();

        try {
            // Main thread attende per 30 secondi (30000 millisecondi)
            Thread.sleep(30000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Termina i thread
        sv.termina();

        try {
            sv.join();
            mt.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("Monitoraggio turbina terminato.");
    }
}

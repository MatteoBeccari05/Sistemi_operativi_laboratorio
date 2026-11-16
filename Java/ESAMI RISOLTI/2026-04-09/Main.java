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

        // Creazione e avvio del thread che simula il battito
        SimulaBattito simulaBattito = new SimulaBattito(pos);
        Thread t1 = new Thread(simulaBattito);
        t1.start();

        // Creazione e avvio del thread che monitora il paziente
        MonitoraPaziente monitoraPaziente = new MonitoraPaziente(pis);
        Thread t2 = new Thread(monitoraPaziente);
        t2.start();

        // Attende 90 secondi prima di terminare il sensore
        try {
            Thread.sleep(90000);
        } catch (Exception e) {
            e.printStackTrace();
        }

        // Terminazione del sensore (chiude anche lo stream)
        simulaBattito.termina();

        // Attende la fine di entrambi i thread
        try {
            t1.join();
            t2.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("Fine");
    }
}

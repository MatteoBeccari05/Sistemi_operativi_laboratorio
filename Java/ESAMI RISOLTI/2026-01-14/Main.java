import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class Main {
	
    public static void main(String[] args) {
        PipedInputStream  pis = new PipedInputStream();
        PipedOutputStream pos;

        try {
            pos = new PipedOutputStream(pis);
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        SimulaLogin   simulaLogin = new SimulaLogin(pos);
        MonitoraLogin monitoraLogin = new MonitoraLogin(pis);

        simulaLogin.start();
        monitoraLogin.start();

        try {
            Thread.sleep(2*60*1000); // 2 minuti
        } catch (Exception e) {
            e.printStackTrace();
        }

        simulaLogin.termina();
        try {
            simulaLogin.join(); // Attendere la terminazione di SimulaLogin
        } catch (Exception e) {
            e.printStackTrace();
        }

        // MonitoraLogin terminerà automaticamente quando la pipe verrà chiusa
        try {
            monitoraLogin.join(); // Attendere la terminazione di MonitoraLogin
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("Monitoraggio login terminato.");
    }
}

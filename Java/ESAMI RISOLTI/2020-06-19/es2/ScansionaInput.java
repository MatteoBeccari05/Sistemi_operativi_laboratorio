package esame.es2;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class ScansionaInput implements Runnable {
    
    final private AtomicBoolean isRunning = new AtomicBoolean(false);
    private PipedInputStream pis = null;
    private VerificaInput verificaInput = null;

    public ScansionaInput(PipedInputStream pis, VerificaInput verifcaInput) {
        this.pis = pis;
        this.verificaInput = verifcaInput;
    }

    public void run() {
        isRunning.set(true);
        BufferedReader br = new BufferedReader(new InputStreamReader(pis));

        while (isRunning.get()) {

            String line = null;
            try {
                line = br.readLine();
            } catch (IOException e1) {
                System.err.println(Thread.currentThread() + " Error in reading from BufferedReader");
                e1.printStackTrace();
            }

            if (line.equals("abcde") || line.equals("1234")) {
                verificaInput.incrementaStringheSospette();
                System.out.println("pericolo");
            } else {
                System.out.println("ok");
            }
        }

    }

    public void stop() {
        isRunning.set(false);
    }
}
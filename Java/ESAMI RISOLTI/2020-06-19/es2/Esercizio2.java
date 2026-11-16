package esame.es2;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class Esercizio2 {
    
    public static void main(String args[]) {
		// creo il canale di comunicazione tra MacchinaA e MacchinaB
		PipedInputStream pis = new PipedInputStream();

		PipedOutputStream pos = null;
		try {
			pos = new PipedOutputStream(pis);
		} catch(IOException ioe) {
			System.err.println("Impossible to create a PipedOutputStream");
			System.exit(-3);
		}

		InputUtente iU = new InputUtente(pos);
		Thread tiU = new Thread(iU);
		tiU.start();

		// creo un'istanza
		VerificaInput verificaInput = new VerificaInput();

		ScansionaInput sI = new ScansionaInput(pis, verificaInput);
		Thread tsI = new Thread(sI);
		tsI.start();


		while(true) {
			try {
				Thread.currentThread().sleep(200);
				// se verifico un numero maggiore di 3 stringhe 
				// vado a terminare i thread
				if (verificaInput.getStringheSospette() > 3) {
					System.out.println("Rilevate > 3 stringhe sopette, termino...");
					iU.stop();
					sI.stop();
					// attendo la terminazione
					tiU.join();
					tsI.join();
					// esco dal ciclo
					break;
				}

			} catch(InterruptedException e) {
				e.printStackTrace();
			}
		}
    }
}
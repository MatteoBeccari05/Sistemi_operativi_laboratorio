package esame.es2;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;


public class Esercizio2 {

    public static void main(String args[]) {

          // creazione della PipedStream per collegare Machine e Quality
          PipedInputStream pis = new PipedInputStream();

          PipedOutputStream pos = null;
          try {
              pos = new PipedOutputStream(pis);
          } catch (IOException e) {
              e.printStackTrace();
              System.exit(-1);
          }

          // Creo un oggetto Overall condiviso dai 
          // due Thread
          Overall overall = new Overall();

          // Creo i due thread
          Machine machine = new Machine(pos);
          Quality quality = new Quality(pis, overall);

          // mando in esecuzione i thread
          machine.start();
          quality.start();

          boolean keepGoing = true; 

          while(keepGoing) {
            try {
                Thread.currentThread().sleep(1000);
                //Controllo numero oggetti difettosi
                if (overall.getDifetti() > overall.getCorretti()) {
                    System.out.println("Numero oggetti difettosi (" + overall.getDifetti() +
                    ") superiore a quelli corretti (" + overall.getCorretti() + ")");

                    System.out.println("Termino I Thread");
                    machine.termina();
                    quality.termina();

                    // setto a false il flag
                    keepGoing = false;
                }

            } catch(InterruptedException e) {
                e.printStackTrace();
            }
          }

          try {
            quality.join();
            machine.join();
          } catch(InterruptedException e) {
              e.printStackTrace();
          }

    }

}
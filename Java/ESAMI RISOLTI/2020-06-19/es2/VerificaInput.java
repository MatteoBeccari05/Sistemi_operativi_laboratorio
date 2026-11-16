package esame.es2;

/**
 * VerificaInput consente una comunicazione thread safe
 * fra InputUtente e ScansionaInput
 */
public class VerificaInput {

    private int stringheSospette = 0;

    public synchronized void incrementaStringheSospette() {
        stringheSospette++;
    }
    
    public synchronized int getStringheSospette() {
        return stringheSospette;
    }
    
}
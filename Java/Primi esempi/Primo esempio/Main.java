public class Main 
{
    public static void main(String args[]) 
    {
        // 1. Creazione di un'istanza della classe Runnable
        EsempioRunnable e = new EsempioRunnable();
        
        // 2. Creazione del Thread passando l'oggetto Runnable
        Thread t = new Thread(e);
        
        // 3. Avvio del thread separato
        t.start();
    }
}
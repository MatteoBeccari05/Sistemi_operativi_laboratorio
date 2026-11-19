import java.util.Random;

public class CounterThread extends Thread 
{
    private Accumulatore a;

    public CounterThread(Accumulatore a)
    {
        this.a = a;
    }

    public void run()
    {
        Random r = new Random();
        double valore = r.nextDouble() * 100;  //numero casuale tra 0 e 100
        a.addValue(valore);
        System.out.println("Thread: " + getName() + " con valore:  " + a.getValue());
    }
}

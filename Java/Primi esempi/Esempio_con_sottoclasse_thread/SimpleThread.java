public class SimpleThread extends Thread
{
    public void run() 
    {
        for(int i=0; i<10; i++) 
        {
            System.out.println(i + " " + getName());
            try 
            {
                // Sospende l'esecuzione per un tempo casuale
                sleep((int) (Math.random() * 1000));
            } 
            catch (InterruptedException e) {}
        }
        System.out.println("DONE!" + getName());
    }
}

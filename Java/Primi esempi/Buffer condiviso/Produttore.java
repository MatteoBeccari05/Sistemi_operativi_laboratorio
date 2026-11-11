class Produttore extends Thread 
{
    private BufferCondiviso buffer;
    
    public Produttore(BufferCondiviso b) 
    {
        this.buffer = b;
    }
    
    public void run() 
    {
        for (int i = 1; i <= 5; i++) 
        {
            buffer.put(i); // Inserisce il numero corrente nel buffer
            try 
            {
                sleep((int) (Math.random() * 1000));
            } 
            catch (InterruptedException e) {}
        }
    }
}
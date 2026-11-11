class Consumatore extends Thread 
{
    private BufferCondiviso buffer;
    
    public Consumatore(BufferCondiviso b) 
    {
        this.buffer = b;
    }
    
    public void run() 
    {
        for (int i = 1; i <= 5; i++) 
        {
            buffer.get(); // Legge il numero dal buffer
            try 
            {
                sleep((int) (Math.random() * 1000));
            } 
            catch (InterruptedException e) {}
        }
    }
}
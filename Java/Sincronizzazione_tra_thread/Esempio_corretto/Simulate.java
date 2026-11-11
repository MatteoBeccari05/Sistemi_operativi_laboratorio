class Simulate 
{
    public static void HWinterrupt() 
    {
        // Nel 50% dei casi, deschedula il thread corrente dalla CPU
        if (Math.random() < 0.5) 
        {
            try 
            {
                Thread.sleep(200);
            } 
            catch(InterruptedException e) {}
        }
    }
}
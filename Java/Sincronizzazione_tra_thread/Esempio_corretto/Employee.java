class Employee extends Thread 
{
    private String _name; 
    private BankAccount _conto;
    private static final int NUM_OF_DEPOSITS = 30;
    private static final int AMOUNT_PER_DEPOSIT = 100;
    
    Employee(String name, BankAccount conto) 
    {
        _name = name;
        _conto = conto;
    }
    
    public void run() 
    {
        try 
        {
            for (int i = 1; i <= NUM_OF_DEPOSITS; ++i) 
            {
                Thread.sleep(500);
                _conto.increase(AMOUNT_PER_DEPOSIT);
            }
            System.out.println("Impiegato " + _name + " ha versato un totale di " + (NUM_OF_DEPOSITS * AMOUNT_PER_DEPOSIT));
        } 
        catch (InterruptedException e) {}
    }
}
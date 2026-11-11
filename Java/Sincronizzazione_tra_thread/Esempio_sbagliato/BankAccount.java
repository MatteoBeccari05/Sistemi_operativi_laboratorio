class BankAccount 
{
    private int value;
    
    BankAccount(int initialValue) 
    {
        value = initialValue;
    }
    
    public void increase(int amount) 
    {
        // Lettura del valore attuale
        int temp = value;
        
        // Simulazione di un'interruzione hardware
        Simulate.HWinterrupt();
        
        // Scrittura del nuovo valore
        value = temp + amount;
        System.out.println("Nuovo saldo: " + value);
    }
}
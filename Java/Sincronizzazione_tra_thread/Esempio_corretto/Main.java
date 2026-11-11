public class Main 
{
    public static void main(String argv[]) 
    {
        // La risorsa condivisa tra i thread
        BankAccount ba = new BankAccount(0);
        
        // Creazione di due thread (impiegati) che agiranno sullo stesso conto
        Employee e1 = new Employee("Mason", ba);
        Employee e2 = new Employee("Steinberg", ba);
        
        e1.start();
        e2.start();
    }
}
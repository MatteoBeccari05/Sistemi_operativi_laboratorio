public class Accumulatore 
{
    private double counter;

    public Accumulatore(double counter)
    {
        this.counter = counter;
    }

    public void addValue(double value)
    {
        this.counter += value;
    }

    public double getValue()
    {
        return this.counter;
    }
}

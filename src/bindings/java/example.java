import com.neueda.cdr.*;

public class example
{
    public static void main(String[] args)
    {

        long field1 = 1;
        long field2 = 2;
        long field3 = 55;

        cdr d = new cdr ();
        d.setInteger (field1, 123);

        System.out.println (d.toString());

        d.setString (field2, "Hello");
        d.setString (field3, "World");

        System.out.println (d.toString ());
        System.out.println (d.getInt32 (field1));
        System.out.println (d.getString (field3));
        
        // this should be a string
        try {
            System.out.println (d.getInt32 (3));
        }
        catch(Exception e)
        {
            System.out.println(e);
        }

        System.out.println("done");
    }
}

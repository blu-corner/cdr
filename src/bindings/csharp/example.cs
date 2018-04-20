/*
 * Copyright 2014-2018 Neueda Ltd.
 */
using System;
using com.neueda.cdr;

class example
{
    static void Main(string[] args)
    {
        long field1 = 1;
        long field2 = 2;
        long field3 = 55;

        cdr d = new cdr ();
        d.setInteger (field1, 123);

        Console.WriteLine (d.toString());

        d.setString (field2, "Hello");
        d.setString (field3, "World");

        Console.WriteLine (d.toString ());
        Console.WriteLine (d.getInt32 (field1));
        Console.WriteLine (d.getString (field3));
        
        // this should be a string
        try {
            Console.WriteLine (d.getInt32 (3));
        }
        catch(Exception e)
        {
            Console.WriteLine(e);
        }

        Console.WriteLine("done");
    }
}

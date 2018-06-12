/*
 * Copyright 2014-2018 Neueda Ltd.
 */
package com.neueda.cdrexample;

import com.neueda.cdr.Cdr;

public class App
{
    public static void main( String[] args )
    {
        Cdr d = new Cdr();
        d.setInteger(1, 100);
        d.setString(2, "hello");
        d.setString(3, "world");

        System.out.println(d.getInteger (1));
        System.out.println(d.getString (2));

        d.setString (2, "Hello");
        d.setString (3, "World");

        System.out.println (d.toString ());
        System.out.println (d.getInt32 (1));
        System.out.println (d.getString (3));
        
        // this should be a string
        try {
            System.out.println (d.getInt32 (3));
        }
        catch(Exception e)
        {
            System.out.println(e);
        }

        System.out.println (d.toString());
    }
}

/*
 * Copyright 2014-2018 Neueda Ltd.
 */
package com.neueda.cdrexample;

import com.neueda.cdr.Cdr;
import com.neueda.cdr.cdrArray;

public class App
{
    public static void main( String[] args )
    {
        Cdr d = new Cdr();
        d.setInteger(1, 100);  // set an integer value
        d.setString(2, "hello");  // set a string value

        // retrieve an integer
        long intVal = d.getInteger (1);
        System.out.println("get integer: " + intVal);

        // retrieve an integer as string
        String strVal = d.getString (1);
        System.out.println("get string: " + strVal);

        // retrieve a string
        strVal = d.getString (2);
        System.out.println("get string: " + strVal);

        // try to retrieve an integer from a key with value that cannot be 
        // converted to integer
        try {
            intVal = d.getInteger(2);
        }
        catch(Exception e)
        {
            System.out.println("retrieve integer from a non-numeric string: " + e.toString());
        }

        // attempt to retrieve a value for non-existant key 
        try {
            intVal = d.getInteger(0);
        }
        catch(Exception e)
        {
            System.out.println("retrieve non existant key: " + e.toString());
        }

        Cdr e = new Cdr();
        e.setString(1, "nested");
        e.setString(2, "cdr");
        d.appendArray(3, e);

        /*
         * NOTE:cdrArray is a cpp typedef std::vector<cdr>. The Swig proxy class
         * extends java.util.AbstractList and so behaves like
         * java.util.ArrayList.
         */
        cdrArray f = d.getArray(3);
        System.out.println("nested-cdr: " + f.get(0).toString());

        System.out.println("cdr.toString(): " + d.toString());
    }
}

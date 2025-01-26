package org.example.model;

public class Menu {
    public void menu(){
        String menu = "1. Get oldest age (Scalar function)\n" +
                "2. Get a student in specific class (JOIN)\n" +
                "3. Get a student's subject by id (ОТВ(CTE))\n" +
                "4. Get metadata\n" +
                "5. Which year for student.(SCALAR FUNCTION FROM 3 LABP)\n" +
                "6. get student by class id (FUNCTION RETURNS TABLE)\n" +
                "7. Call procedure to create student\n" +
                "8. Get the current date (CALL SYSTEM FUNCTION)\n" +
                "9. Create new table\n" +
                "10. Insert data into new table\n" +
                "11. Define teacher information subject (defends)";

        System.out.println("\n\n");
        System.out.println(menu);
    }
}

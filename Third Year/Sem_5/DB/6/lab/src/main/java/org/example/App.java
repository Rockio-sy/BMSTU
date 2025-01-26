package org.example;

import org.example.model.Menu;
import org.example.repository.Connect;
import org.example.repository.StudentRepository;

import java.sql.SQLException;
import java.util.Scanner;

public class App {
    // Static dependencies
    static Menu menu = new Menu();
    static Connect connect = new Connect();
    static StudentRepository studentRepository = new StudentRepository(connect);

    public static void main(String[] args) throws SQLException {
        Scanner read = new Scanner(System.in);

        while (true) {
            // Display menu options
            menu.menu();
            System.out.println("Choose an option (or 0 to exit):");

            // Read user input
            if (!read.hasNextInt()) {
                System.out.println("Invalid input. Please enter a number.");
                read.next(); // Clear invalid input
                continue;
            }

            int option = read.nextInt();

            // Exit the loop if the user chooses 0
            if (option == 0) {
                System.out.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<IN JVM WE TRUST>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
                break;
            }

            // Handle menu options
            switch (option) {
                case 1:
                    // Example: Get the oldest student
                    try {
                        System.out.println("-----Result-----");
                        System.out.println(studentRepository.getOldest());
                    } catch (SQLException e) {
                        System.err.println("Error retrieving the oldest student: " + e.getMessage());
                    }
                    break;

                case 2:
                    // Example: Get students from a specific class
                    System.out.println("Insert the class ID:");
                    if (read.hasNextInt()) {
                        int classId = read.nextInt();
                        try {
                            System.out.println("-----Result-----");
                            System.out.println(studentRepository.getFromClass(Integer.toUnsignedLong(classId)));
                        } catch (SQLException e) {
                            System.err.println("Error retrieving students from class: " + e.getMessage());
                        }
                    } else {
                        System.out.println("Invalid class ID. Please try again.");
                        read.next(); // Clear invalid input
                    }
                    break;

                case 3:
                    System.out.println("Insert the student id:");
                    studentRepository.thirdTask(Integer.toUnsignedLong(read.nextInt()));
                    break;

                case 4:
                    System.out.println("-----Result-----");
                    studentRepository.getMetaData();
                    break;

                case 5:
                    System.out.println("Insert the student id:");
                    Long id = read.nextLong();
                    studentRepository.callBirthYearFunction(id);
                    break;

                case 6:
                    System.out.println("Insert class id:");
                    int cId = read.nextInt();
                    System.out.println("-----Result-----");
                    studentRepository.getStudentsByClass(cId);
                    break;

                case 7:
                    System.out.println("Insert first name:");
                    String firstName = read.next();
                    System.out.println("Insert last name:");
                    String lastName = read.next();
                    System.out.println("Insert study year:");
                    int studyYear = read.nextInt();
                    System.out.println("Insert class group:");
                    String classGroup = read.next();
                    System.out.println("Insert age:");
                    int age = read.nextInt();
                    studentRepository.createProcedure(firstName, lastName, studyYear, classGroup, age);
                    break;

                case 8:
                    studentRepository.getCurrentDate();
                    break;

                case 9:
                    studentRepository.createTable();
                    break;

                case 10:
                    studentRepository.insertIntoNewTable();
                    break;

                case 11:
                    studentRepository.getDefends();
                    break;
                default:
                    System.out.println("Invalid option. Please choose a valid menu item.");
                    break;
            }
        }

        // Close the scanner
        read.close();
    }
}

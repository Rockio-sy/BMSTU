package org.example;

import com.fasterxml.jackson.core.JsonProcessingException;
import org.hibernate.boot.model.internal.CreateKeySecondPass;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Scanner;

public class App {
    static Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {
        Repository repository = new Repository();
        Service service = new Service(repository);


        boolean running = true;

        while (running) {
            // Main menu
            System.out.println("Main Menu: Select a task");
            System.out.println("1. Task 1: LINQ to Object");
            System.out.println("2. Task 2: LINQ to XML/JSON");
            System.out.println("3. Task 3: LINQ to SQL");
            System.out.println("4. Exit");

            int taskOption = scanner.nextInt();

            switch (taskOption) {
                case 1:
                    showTask1Menu(service, scanner);
                    break;
                case 2:
                    showTask2Menu(service, scanner, repository);
                    break;
                case 3:
                    showTask3Menu(service, scanner, repository);
                    break;
                case 4:
                    running = false; // Exit the program
                    System.out.println("Exiting the program...");
                    break;
                default:
                    System.out.println("Invalid option. Please try again.");
            }
        }

        scanner.close();
    }

    // Menu for Task 1: LINQ to Object
    private static void showTask1Menu(Service service, Scanner scanner) {
        boolean task1Running = true;
        while (task1Running) {
            System.out.println("\nTask 1 Menu: LINQ to Object");
            System.out.println("1. Filter by Study Year");
            System.out.println("2. Sort by Age");
            System.out.println("3. Map to Full Names");
            System.out.println("4. Group by Class Group");
            System.out.println("5. Count Students by Class Group");
            System.out.println("6. Back to Main Menu");

            int option = scanner.nextInt();

            switch (option) {
                case 1:
                    System.out.println("Filtering by Study Year...");
                    service.getByStudyYear(2); // Example: Study Year 2
                    break;
                case 2:
                    System.out.println("Sorting by Age...");
                    service.sortByAge();
                    break;
                case 3:
                    System.out.println("Mapping to Full Names...");
                    service.mapToFullName();
                    break;
                case 4:
                    System.out.println("Grouping by Class Group...");
                    service.groupByClassGroup();
                    break;
                case 5:
                    System.out.println("Counting by Class Group...");
                    service.countStudentsByClassGroup();
                    break;
                case 6:
                    task1Running = false; // Go back to the main menu
                    break;
                case 7:
                    List<Teacher> teachers = service.getDefends();
                    for(Teacher t : teachers){
                        System.out.printf("%9.20s | %9.20s\n", t.getFirstName(), t.getSubject());
                    }
                default:
                    System.out.println("Invalid option. Please try again.");
            }
        }
    }

    // Menu for Task 2: LINQ to XML/JSON
    private static void showTask2Menu(Service service, Scanner scanner, Repository repository) {
        boolean task2Running = true;
        while (task2Running) {
            System.out.println("\nTask 2 Menu: LINQ to XML/JSON");
            System.out.println("1. Read from XML/JSON");
            System.out.println("2. Update XML/JSON");
            System.out.println("3. Add to XML/JSON");
            System.out.println("4. Back to Main Menu");

            int option = scanner.nextInt();

            switch (option) {
                case 1:
                    System.out.println("Reading from XML/JSON...");
                    try {
                        String json = service.convertToJson(repository.getAll());
                        service.readJson(json);
                    } catch (JsonProcessingException e) {
                        System.out.println(e.getMessage());
                    }
                    break;
                case 2:
                    System.out.println("Updating XML/JSON...");
                    try {
                        String json = service.convertToJson(repository.getAll());
                        System.out.println("Enter ID:");
                        int id = scanner.nextInt();
                        System.out.println("Enter New fist name");
                        String newFirstName = scanner.next();
                        String response = service.updateStudentInJson(json, id, newFirstName);
                        System.out.println(response);
                        System.out.println("\n\nDO SEARCH BY ctrl+F then id\"=ID");
                    } catch (JsonProcessingException e) {
                        System.out.println(e.getMessage());
                    }
                    break;
                case 3:
                    try {
                        String json = service.convertToJson(repository.getAll());
                        System.out.println("Insert First Name:");
                        String firstName = scanner.next();
                        System.out.println("Insert last name");
                        String lastName = scanner.next();
                        System.out.println("Insert study year");
                        int studyYear = scanner.nextInt();
                        System.out.println("Insert class group");
                        String classGroup = scanner.next();
                        System.out.println("Insert age");
                        int age = scanner.nextInt();

                        Student toAdd = new Student(0, firstName, lastName, studyYear, classGroup, LocalDateTime.now(), LocalDateTime.now(), age);
                        System.out.println("Adding to XML/JSON...");
                        String response = service.addStudentToJson(json, toAdd);
                        System.out.println(response);
                    } catch (JsonProcessingException e) {
                        System.out.println(e.getMessage());
                    }
                    break;
                case 4:
                    task2Running = false;
                    break;
                default:
                    System.out.println("Invalid option. Please try again.");
            }
        }
    }

    // Menu for Task 3: LINQ to SQL
    private static void showTask3Menu(Service service, Scanner scanner, Repository repository) {
        boolean task3Running = true;
        while (task3Running) {
            System.out.println("\nTask 3 Menu: LINQ to SQL");
            System.out.println("1. Single Table Query");
            System.out.println("2. Multi-table Query");
            System.out.println("3. Add Data");
            System.out.println("4. Update Data");
            System.out.println("5. Delete Data");
            System.out.println("6. Call Stored Procedure");
            System.out.println("7. Back to Main Menu");

            int option = scanner.nextInt();

            switch (option) {
                case 1:
                    System.out.println("Performing Single Table Query...");
                    service.printStudents(repository.getAll());
                    break;
                case 2:
                    System.out.println("Performing Multi-table Query...");
                    service.printStudents(repository.getAllStudentsWithClasses());
                    break;
                case 3:
                    System.out.print("Enter first name: ");
                    String firstName = scanner.next();
                    System.out.print("Enter last name: ");
                    String lastName = scanner.next();
                    System.out.print("Enter study year: ");
                    int newStudyYear = scanner.nextInt();
                    System.out.print("Enter class group: ");
                    String classGroup = scanner.next();
                    System.out.print("Enter age: ");
                    int age = scanner.nextInt();
                    System.out.println("Adding Data...");
                    service.addNewStudent(firstName, lastName, newStudyYear, classGroup, age);
                    service.getStudentsByFirstName(firstName);

                    break;
                case 4:
                    System.out.print("Enter student ID to update: ");
                    int studentId = scanner.nextInt();
                    System.out.print("Enter new study year: ");
                    int updateStudyYear = scanner.nextInt();
                    System.out.println("Updating Data...");
                    service.updateStudentStudyYear(studentId, updateStudyYear);
                    service.getStudentById(studentId);
                    break;
                case 5:
                    System.out.print("Enter student ID to delete: ");
                    int deleteId = scanner.nextInt();
                    System.out.println("Deleting Data...");
                    service.deleteStudent(deleteId);
                    break;
                case 6:
                    System.out.println("Enter table name:");
                    String tableName = scanner.next();
                    System.out.println("Calling Stored Procedure...");
                    service.getTableMetadata(tableName);
                    break;
                case 7:
                    task3Running = false; // Go back to the main menu
                    break;
                default:
                    System.out.println("Invalid option. Please try again.");
            }
        }
    }
}

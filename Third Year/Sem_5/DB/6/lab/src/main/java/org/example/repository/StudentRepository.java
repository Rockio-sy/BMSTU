package org.example.repository;

import org.example.model.Student;

import java.sql.*;
import java.util.Scanner;

public class StudentRepository {

    private final Connect connect;

    public StudentRepository(Connect connect) {
        this.connect = connect;
    }

    public int getOldest() throws SQLException {
        int result = 0;
        String sql = "SELECT MAX(age) age FROM students;";
        try (Connection connection = connect.connect()) {
            PreparedStatement stmt = connection.prepareStatement(sql);
            ResultSet rs = stmt.executeQuery();
            while (rs.next()) {
                result = rs.getInt("age");
            }
        }
        return result;
    }

    public Student getFromClass(Long classNumber) throws SQLException {
        String sql = "SELECT * FROM students s " +
                "JOIN stud_class sc ON sc.student_id = s.id " +
                "JOIN classes c ON c.id = sc.class_id " +
                "WHERE c.id = ?";
        try (Connection connection = connect.connect()) {
            PreparedStatement stmt = connection.prepareStatement(sql);
            Student resp = null;
            stmt.setLong(1, classNumber);
            ResultSet rs = stmt.executeQuery();
            while (rs.next()) {
                resp = new Student(rs.getLong("id"), rs.getString("first_name")
                        , rs.getString("last_name"), rs.getInt("study_year"), rs.getString("class_group"),
                        rs.getInt("age"));
            }
            return resp;
        }
    }

    public void thirdTask(Long studentId) {
        String sql = "with temp_table as (" +
                "select s.id, s.first_name," +
                " c.subject from classes c join stud_class sc on c.id = sc.class_id join students s on s.id = sc.student_id)" +
                " select distinct * from temp_table where id = ?;";

        try (Connection connection = connect.connect()) {
            PreparedStatement stmt = connection.prepareStatement(sql);
            stmt.setLong(1, studentId);
            ResultSet rs = stmt.executeQuery();
            System.out.println("----Results----");
            while (rs.next()) {
                Long id = rs.getLong("id");
                String firstName = rs.getString("first_name");
                String subject = rs.getString("subject");
                System.out.printf("ID: %d \nName: %s\nSubject:%s", id, firstName, subject);
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public void getMetaData() throws SQLException {
        String sql = " SELECT COLUMN_NAME, DATA_TYPE FROM  INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'students';";
        try (Connection connection = connect.connect()) {
            PreparedStatement stmt = connection.prepareStatement(sql);
            ResultSet rs = stmt.executeQuery();
            while (rs.next()) {
                String columnName = rs.getString("column_name");
                String dataType = rs.getString("data_type");
                System.out.printf("%-30s%-20s\n", columnName, dataType);
            }
        }
    }

    public void createProcedure(String firstName, String lastName, int studyYear, String group, int age) throws SQLException {
        String sql = "CALL add_student(?, ?, ?, ?, ?);";
        try (Connection connection = connect.connect()) {
            CallableStatement stmt = connection.prepareCall(sql);
            stmt.setString(1, firstName);
            stmt.setString(2, lastName);
            stmt.setInt(3, studyYear);
            stmt.setString(4, group);
            stmt.setInt(5, age);
            stmt.execute();
            System.out.println("CALLED");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public void createTable() {
        String createTableSQL =
                "CREATE TABLE IF NOT EXISTS courses (" +
                        "    id SERIAL PRIMARY KEY,               -- Unique course ID, automatically incremented\n" +
                        "    course_name VARCHAR(100) NOT NULL,    -- Course name, maximum 100 characters\n" +
                        "    course_code VARCHAR(20) NOT NULL,     -- Unique course code, e.g., CS101\n" +
                        "    description TEXT,                     -- Description of the course\n" +
                        "    credits INT NOT NULL CHECK (credits > 0), -- Number of credits for the course, must be greater than 0\n" +
                        "    department VARCHAR(50),               -- Department offering the course\n" +
                        "    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Automatically set to current time when a row is created\n" +
                        "    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- Automatically set to current time when a row is created\n" +
                        ");";

        try (Connection connection = connect.connect();
             Statement stmt = connection.createStatement()) {
            // Execute the CREATE TABLE query
            stmt.executeUpdate(createTableSQL);
            System.out.println("Table 'courses' created successfully.");
        } catch (SQLException e) {
            // Handle SQL exceptions
            System.err.println("Error while creating table: " + e.getMessage());
        }
    }

    public void insertIntoNewTable() {
        // SQL statement for inserting data into the 'courses' table
        String insertSQL = "INSERT INTO courses (course_name, course_code, description, credits, department) VALUES (?, ?, ?, ?, ?)";

        // Using Scanner to get input from the user
        try (Connection connection = connect.connect();
             PreparedStatement stmt = connection.prepareStatement(insertSQL);
             Scanner scanner = new Scanner(System.in)) {

            // Get user input
            System.out.print("Enter Course Name: ");
            String courseName = scanner.nextLine();

            System.out.print("Enter Course Code (e.g., CS101): ");
            String courseCode = scanner.nextLine();

            System.out.print("Enter Course Description: ");
            String description = scanner.nextLine();

            System.out.print("Enter Credits (INTEGER): ");
            int credits = scanner.nextInt();
            scanner.nextLine();  // Consume the newline left by nextInt()

            System.out.print("Enter Department(STRING): ");
            String department = scanner.nextLine();
            scanner.close();

            // Set the values for the prepared statement
            stmt.setString(1, courseName);
            stmt.setString(2, courseCode);
            stmt.setString(3, description);
            stmt.setInt(4, credits);
            stmt.setString(5, department);

            // Execute the insert statement
            int rowsAffected = stmt.executeUpdate();
            System.out.println("Inserted " + rowsAffected + " row(s) into the 'courses' table.");
        } catch (SQLException e) {
            // Handle SQL exceptions
            System.err.println("Error while inserting data: " + e.getMessage());
        }
    }

    public void callBirthYearFunction(Long studentId) {
        // SQL query to call the scalar function "calculate_birth_year"
        String sql = "SELECT calculate_birth_year(age) FROM students WHERE id = ?";

        try (Connection connection = connect.connect()) {
            // Prepare the statement with the SQL query
            PreparedStatement stmt = connection.prepareStatement(sql);

            // Set the student ID parameter
            stmt.setLong(1, studentId);

            // Execute the query and get the result
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                // Retrieve the result of the scalar function (birth year)
                int birthYear = rs.getInt(1); // The result is in the first column of the result set
                System.out.println("The student's birth year is: " + birthYear);
            } else {
                System.out.println("No student found with the given ID.");
            }
        } catch (SQLException e) {
            System.err.println("Error calling the scalar function: " + e.getMessage());
        }
    }

    public void getCurrentDate() throws SQLException {
        String sql = "SELECT current_date";  // System function to get the current date

        try (Connection connection = connect.connect()) {
            PreparedStatement stmt = connection.prepareStatement(sql);
            ResultSet rs = stmt.executeQuery();

            if (rs.next()) {
                // Retrieve the current date from the result set
                Date currentDate = rs.getDate(1);
                System.out.println("Current date: " + currentDate);
            }
        } catch (SQLException e) {
            System.err.println("Error calling the system function: " + e.getMessage());
        }
    }

    public void getStudentsByClass(int classId) throws SQLException {
        String sql = "SELECT * FROM get_student_by_class(?)";

        try (Connection connection = connect.connect();
             PreparedStatement stmt = connection.prepareStatement(sql)) {

            // Set the class ID in the query
            stmt.setInt(1, classId);

            // Execute the query and retrieve the results
            ResultSet rs = stmt.executeQuery();

            // Process the result set
            while (rs.next()) {
                int studentId = rs.getInt("student_id");
                String firstName = rs.getString("first_name");
                String lastName = rs.getString("last_name");
                int age = rs.getInt("age");
                System.out.println("Student ID: " + studentId + ", Name: " + firstName + " " + lastName + ", Age: " + age);
            }
        } catch (SQLException e) {
            System.out.println("Error while calling the function: " + e.getMessage());
        }
    }

    public void getDefends() {
        String sql = "SELECT * FROM teachers WHERE subject = 'Computer Science'";
        try(Connection connection = connect.connect()){
            PreparedStatement stmt = connection.prepareStatement(sql);
            ResultSet rs = stmt.executeQuery();
            while(rs.next()){
                String teacherName = rs.getString("first_name");
                String subject = rs.getString("subject");
                System.out.printf("%9.10s | %9.20s\n", teacherName, subject);
            }
        }catch (SQLException e){
            System.out.println(e.getMessage());
        }
    }
}

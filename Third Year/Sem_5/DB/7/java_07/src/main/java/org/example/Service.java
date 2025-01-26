package org.example;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsr310.JavaTimeModule;

import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.stream.Collectors;

public class Service {
    private final Repository repository;

    public Service(Repository repository) {
        this.repository = repository;
    }

    public void getByStudyYear(int studyYear) {
        List<Student> data = repository.getAll();
        // Correct the filter condition to match studyYear, not age
        List<Student> response = data.stream()
                .filter(student -> student.getStudyYear() == studyYear)
                .toList();

        // Print out the students
        printStudents(response);
    }

    public void getById(int id) {
        Student student = repository.getById(id);
        System.out.println(student);
    }


    public void sortByAge() {
        List<Student> data = repository.getAll();
        List<Student> response = data.stream()
                .sorted(Comparator.comparingInt(Student::getAge))
                .toList();
        printStudents(response);
    }


    public void mapToFullName() {
        List<Student> data = repository.getAll();
        List<String> response = data.stream()
                .map(student -> STR."\{student.getFirstName()} \{student.getLastName()}")
                .toList();

        for (String fullName : response) {
            System.out.println(fullName);
        }
    }

    public void groupByClassGroup() {
        List<Student> data = repository.getAll();
        Map<String, List<Student>> response = data.stream()
                .collect(Collectors.groupingBy(Student::getClassGroup));

        response.forEach((classGroup, studentInClass) -> {
            System.out.println(STR."Class group: \{classGroup}");
            printStudents(studentInClass);
        });
    }

    public void countStudentsByClassGroup() {
        List<Student> students = repository.getAll();
        Map<String, Long> studentCountByGroup = students.stream()
                .collect(Collectors.groupingBy(Student::getClassGroup, Collectors.counting()));

        studentCountByGroup.forEach((classGroup, count) -> {
            System.out.println("Class Group: " + classGroup + ", Student Count: " + count);
        });
    }


    public String convertToJson(List<Student> list) throws JsonProcessingException {
        ObjectMapper mapper = new ObjectMapper();
        mapper.registerModule(new JavaTimeModule());
        return mapper.writeValueAsString(list);
    }

    public void readJson(String json) {
        ObjectMapper mapper = new ObjectMapper();
        mapper.registerModule(new JavaTimeModule());
        try {
            List<Student> students = mapper.readValue(json, mapper.getTypeFactory().constructCollectionType(List.class, Student.class));
            students.forEach(System.out::println); // Print the students
        } catch (Exception e) {
            System.out.println(e.getMessage());

        }
    }

    public String updateStudentInJson(String json, int studentId, String newFirstName) {
        ObjectMapper mapper = new ObjectMapper();
        mapper.registerModule(new JavaTimeModule());
        try {
            // Convert JSON back to List
            List<Student> students = mapper.readValue(json, mapper.getTypeFactory().constructCollectionType(List.class, Student.class));

            // Find and update the student by ID
            for (Student student : students) {
                if (student.getId() == studentId) {
                    student.setFirstName(newFirstName); // Update first name
                }
            }

            // Convert the list back to JSON
            return mapper.writeValueAsString(students);
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }

    public String addStudentToJson(String json, Student newStudent) {
        ObjectMapper mapper = new ObjectMapper();
        mapper.registerModule(new JavaTimeModule());
        try {
            // Convert JSON back to List
            List<Student> students = mapper.readValue(json, mapper.getTypeFactory().constructCollectionType(List.class, Student.class));

            // Add the new student to the list
            students.add(newStudent);

            // Convert the list back to JSON
            return mapper.writeValueAsString(students);
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }

    public void updateStudentStudyYear(int studentId, int newStudyYear) {
        repository.updateStudentStudyYear(studentId, newStudyYear);
    }

    public void addNewStudent(String firstName, String lastName, int studyYear, String classGroup, int age) {
        Student student = new Student();
        student.setFirstName(firstName);
        student.setLastName(lastName);
        student.setStudyYear(studyYear);
        student.setClassGroup(classGroup);
        student.setAge(age);
        repository.addStudent(student);
    }

    public void deleteStudent(int studentId) {
        repository.deleteStudentById(studentId);
    }

    public void getTableMetadata(String tableName) {
        List<Object[]> metadata = repository.callProcedureGetTableMetadata(tableName);

        if (metadata == null || metadata.isEmpty()) {
            System.out.println("No metadata found for table: " + tableName);
            return;
        }

        System.out.printf("%-20s %-20s %-10s\n", "Column Name", "Data Type", "Nullable");
        for (Object[] row : metadata) {
            System.out.printf("%-20s %-20s %-10s\n", row[0], row[1], row[2]);
        }
    }


    public void getStudentsByFirstName(String firstName) {
        List<Student> students = repository.getStudentsByFirstName(firstName);
        printStudents(students);
    }
    public void getStudentById(int id) {
        Student student = repository.getStudentById(id);
        if (student != null) {
            printStudent(student);
        } else {
            System.out.println("No student found with ID: " + id);
        }
    }
    public void printStudent(Student student) {
        System.out.printf("%-5s %-15s %-15s %-5s %-10s %-15s\n",
                "ID", "First Name", "Last Name", "Age", "Study Year", "Class Group");
        System.out.printf("%-5d %-15s %-15s %-5d %-10d %-15s\n",
                student.getId(), student.getFirstName(), student.getLastName(),
                student.getAge(), student.getStudyYear(), student.getClassGroup());
    }


    public void printStudents(List<Student> students) {
        // Print table headers
        System.out.printf("%-5s %-20s %-20s %-10s %-15s %-10s%n",
                "ID", "First Name", "Last Name", "Age", "Class Group", "Study Year");
        System.out.println("--------------------------------------------------------------------------------");

        // Print each student's details in rows
        for (Student student : students) {
            System.out.printf("%-5d %-20s %-20s %-10d %-15s %-10d%n",
                    student.getId(),
                    student.getFirstName(),
                    student.getLastName(),
                    student.getAge(),
                    student.getClassGroup(),
                    student.getStudyYear());
        }
    }

    public List<Teacher> getDefends() {
        return repository.getDefends();
    }
}

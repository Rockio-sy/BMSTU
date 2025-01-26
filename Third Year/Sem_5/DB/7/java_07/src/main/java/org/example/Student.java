package org.example;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import org.hibernate.annotations.ColumnDefault;

import java.time.LocalDateTime;
import java.util.List;

@Entity
@Table(name = "students")
public class Student {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    @Column(name = "first_name", nullable = false)
    private String firstName;
    @Column(name = "last_name", nullable = false)
    private String lastName;
    @Column(name = "study_year", nullable = false)
    private int studyYear;
    @Column(name = "class_group", nullable = false)
    private String classGroup;
    @Column(name = "created_at")
    private LocalDateTime createdAt;

    @Column(name = "updated_at")
    private LocalDateTime updatedAt;

    @Column(name = "age", nullable = false)
    private int age;

    @OneToMany(mappedBy = "student")
    @JsonIgnore
    private List<StudentClass> studentClasses; // Link to the join table
    public Student(){}
    // Constructor
    public Student(int id, String firstName, String lastName, int studyYear, String classGroup,
                   LocalDateTime createdAt, LocalDateTime updatedAt, int age) {
        this.id = id;
        this.firstName = firstName;
        this.lastName = lastName;
        this.studyYear = studyYear;
        this.classGroup = classGroup;
        this.createdAt = createdAt;
        this.updatedAt = updatedAt;
        this.age = age;
    }

    public List<StudentClass> getStudentClasses() {
        return studentClasses;
    }

    public void setStudentClasses(List<StudentClass> studentClasses) {
        this.studentClasses = studentClasses;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public int getStudyYear() {
        return studyYear;
    }

    public void setStudyYear(int studyYear) {
        this.studyYear = studyYear;
    }

    public String getClassGroup() {
        return classGroup;
    }

    public void setClassGroup(String classGroup) {
        this.classGroup = classGroup;
    }

    public LocalDateTime getCreatedAt() {
        return createdAt;
    }

    public void setCreatedAt(LocalDateTime createdAt) {
        this.createdAt = createdAt;
    }

    public LocalDateTime getUpdatedAt() {
        return updatedAt;
    }

    public void setUpdatedAt(LocalDateTime updatedAt) {
        this.updatedAt = updatedAt;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return STR."Student{id=\{id}, firstName='\{firstName}\{'\''}, lastName='\{lastName}\{'\''}, studyYear=\{studyYear}, classGroup='\{classGroup}\{'\''}, createdAt=\{createdAt}, updatedAt=\{updatedAt}, age=\{age}\{'}'}";
    }
}

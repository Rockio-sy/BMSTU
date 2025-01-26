package org.example;


public class Student {
    private long id;
    private String firstName;
    private String lastName;
    private int studyYear;
    private String classGroup;
    private int age;

    public Student(long id, String firstName, String lastName, int studyYear, String classGroup, int age) {
        this.id = id;
        this.firstName = firstName;
        this.lastName = lastName;
        this.studyYear = studyYear;
        this.classGroup = classGroup;
        this.age = age;
    }

    public Student(String firstName, String lastName, int age){
        this.firstName = firstName;
        this.lastName = lastName;
        this.age = age;
    }
    public long getId() {
        return id;
    }

    public void setId(long id) {
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

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return "Student{" +
                "id=" + id +
                ", firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", studyYear=" + studyYear +
                ", classGroup='" + classGroup + '\'' +
                ", age=" + age +
                '}';
    }
}

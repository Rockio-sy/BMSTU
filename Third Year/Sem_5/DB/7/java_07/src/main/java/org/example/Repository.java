package org.example;

import jakarta.persistence.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;
import java.util.List;

public class Repository {
    private static SessionFactory factory ;

    static {
        // Set up Hibernate session factory
        factory = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Student.class)
                .addAnnotatedClass(Class.class)
                .addAnnotatedClass(StudentClass.class)
                .addAnnotatedClass(Teacher.class)
                .buildSessionFactory();
    }

    public List<Student> getAll() {
        try (Session session = factory.getCurrentSession()) {

            session.beginTransaction();
            // Use the correct class name for the query
            List<Student> students = session.createQuery("from Student", Student.class).getResultList();
            session.getTransaction().commit();
            return students;
        }
    }

    public Student getById(int id){
        try(Session session = factory.getCurrentSession()){
            session.beginTransaction();
            Student student = session.get(Student.class, id);
            session.getTransaction().commit();
            return student;
        }
    }

    public void save(Student student) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            session.save(student);  // Save the student
            session.getTransaction().commit();
        }
    }

    // Update an existing student
    public void update(Student student) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            session.merge(student);  // Update the student
            session.getTransaction().commit();
        }
    }

    // Delete a student by ID
    public void delete(int id) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            Student student = session.get(Student.class, id);  // Get the student by ID
            if (student != null) {
                // Delete the student
                session.remove(student);
            }
            session.getTransaction().commit();
        }
    }

    // Get students by study year
    public List<Student> getByStudyYear(int studyYear) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            List<Student> students = session.createQuery("from Student s where s.studyYear = :studyYear", Student.class)
                    .setParameter("studyYear", studyYear)
                    .getResultList();
            session.getTransaction().commit();
            return students;
        }
    }

    // Custom query example (select by class group)
    public List<Student> getByClassGroup(String classGroup) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            List<Student> students = session.createQuery("from Student s where s.classGroup = :classGroup", Student.class)
                    .setParameter("classGroup", classGroup)
                    .getResultList();
            session.getTransaction().commit();
            return students;
        }
    }

    public List<Student> getAllStudentsWithClasses() {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();

            // HQL query to join Student, StudentClass, and Class
            String hql = "SELECT s FROM Student s " +
                    "JOIN FETCH s.studentClasses sc " +
                    "JOIN FETCH sc.clazz c";
            List<Student> students = session.createQuery(hql, Student.class).getResultList();

            session.getTransaction().commit();
            return students;
        }
    }

    public void addStudent(Student student) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            session.persist(student);
            session.getTransaction().commit();
        }
    }

    public void updateStudentStudyYear(int studentId, int newStudyYear) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            Student student = session.get(Student.class, studentId);
            if (student != null) {
                student.setStudyYear(newStudyYear);
            }
            session.getTransaction().commit();
        }
    }

    public void deleteStudentById(int studentId) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            Student student = session.get(Student.class, studentId);
            if (student != null) {
                session.remove(student);
            }
            session.getTransaction().commit();
        }
    }


    public List<Object[]> callProcedureGetTableMetadata(String tableName) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();

            // Call the PostgreSQL function using the proper SQL query
            Query query = session.createNativeQuery("SELECT * FROM get_table_metadata(:tableName)");
            query.setParameter("tableName", tableName);  // Pass the table name as parameter

            // Execute the query and get the result list
            List<Object[]> results = query.getResultList();

            // Print each row of results
            for (Object[] row : results) {
                String columnName = (String) row[0];
                String dataType = (String) row[1];
                Boolean isNotNull = (Boolean) row[2];

                System.out.println("Column Name: " + columnName + ", Data Type: " + dataType + ", Is Not Null: " + isNotNull);
            }

            session.getTransaction().commit();
            return results;
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }




    public List<Student> getStudentsByFirstName(String firstName) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            String hql = "FROM Student WHERE firstName = :firstName";
            List<Student> students = session.createQuery(hql, Student.class)
                    .setParameter("firstName", firstName)
                    .getResultList();
            session.getTransaction().commit();
            return students;
        }
    }

    public Student getStudentById(int id) {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();
            Student student = session.get(Student.class, id);
            session.getTransaction().commit();
            return student;
        }
    }

    public List<Teacher> getDefends() {
        try (Session session = factory.getCurrentSession()) {
            session.beginTransaction();

            // Query the database using HQL
            List<Teacher> teachers = session.createQuery("from Teacher where subject = 'Computer Science'", Teacher.class)
                    .getResultList();

            session.getTransaction().commit();
            return teachers;
        }
    }

}

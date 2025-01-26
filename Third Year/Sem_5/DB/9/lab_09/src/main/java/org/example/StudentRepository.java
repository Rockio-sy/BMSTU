package org.example;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class StudentRepository {
    private final Connect connect;

    public StudentRepository(Connect connect) {
        this.connect = connect;
    }

    public List<Student> getTopByAge() {
        List<Student> top = new ArrayList<>();
        String sql = "SELECT * FROM students ORDER BY age DESC LIMIT 10";
        try (Connection connection = connect.connect()) {
            PreparedStatement prepared = connection.prepareStatement(sql);
            ResultSet rs = prepared.executeQuery();
            while (rs.next()) {
                top.add(new Student(rs.getString("first_name"), rs.getString("last_name"), rs.getInt("age")));
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        return top;
    }

    public List<HashMap<String, Object>> getTotalByClassGroup() {
        List<HashMap<String, Object>> total = new ArrayList<>();
        String sql = "SELECT class_group, COUNT(*) as total FROM students GROUP BY class_group ORDER BY total DESC";
        try (Connection connection = connect.connect()) {
            PreparedStatement prepared = connection.prepareStatement(sql);
            ResultSet rs = prepared.executeQuery();
            while (rs.next()) {
                HashMap<String, Object> cur = new HashMap<>();
                cur.put("Class group", rs.getString("class_group"));
                cur.put("total", (rs.getInt("total")));
                total.add(cur);
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        return total;
    }

    public void addRandomStudent() {
        String sql = "INSERT INTO students (first_name, last_name, study_year, class_group, age) " +
                "VALUES (?, ?, ?, ?, ?)";
        try (Connection connection = connect.connect()) {
            PreparedStatement statement = connection.prepareStatement(sql);
            statement.setString(1, "John" + (int) (Math.random() * 100));
            statement.setString(2, "Doe" + (int) (Math.random() * 100));
            statement.setInt(3, (int) (Math.random() * 4) + 1); // Study year between 1 and 4
            statement.setString(4, "Group" + (int) (Math.random() * 10)); // Random class group
            statement.setInt(5, (int) (Math.random() * 25) + 18); // Random age between 18 and 42
            statement.executeUpdate();
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public void deleteRandomStudent() {
        String sql = "DELETE FROM students WHERE id = (SELECT id FROM students ORDER BY random() LIMIT 1)";
        try (Connection connection = connect.connect()) {
            PreparedStatement statement = connection.prepareStatement(sql);
            statement.executeUpdate();
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }


    public void updateRandomStudent() {
        String sql = "UPDATE students " +
                "SET age = age + 1, class_group = CONCAT(class_group, '_Updated') " +
                "WHERE id = (SELECT id FROM students ORDER BY random() LIMIT 1)";
        try (Connection connection = connect.connect()) {
            PreparedStatement statement = connection.prepareStatement(sql);
            statement.executeUpdate();
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

}

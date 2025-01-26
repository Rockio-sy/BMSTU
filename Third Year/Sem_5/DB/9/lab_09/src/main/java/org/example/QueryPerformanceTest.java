package org.example;

import org.example.StudentRepository;
import org.example.StudentRepositoryWithCache;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class QueryPerformanceTest {

    private final StudentRepository studentRepository;
    private final StudentRepositoryWithCache repoWithCache;

    public QueryPerformanceTest(StudentRepository studentRepository, StudentRepositoryWithCache repoWithCache) {
        this.studentRepository = studentRepository;
        this.repoWithCache = repoWithCache;
    }

    public void startPerformanceTest() {
        ScheduledExecutorService executorService = Executors.newScheduledThreadPool(3);

        // Uncached queries
        executorService.scheduleAtFixedRate(() -> {
            long startTime = System.currentTimeMillis();
            studentRepository.getTotalByClassGroup();
            long endTime = System.currentTimeMillis();
            System.out.println("Uncached Query Time: " + (endTime - startTime) + " ms");
        }, 0, 5, TimeUnit.SECONDS);

        // Cached queries
        executorService.scheduleAtFixedRate(() -> {
            long startTime = System.currentTimeMillis();
            repoWithCache.getTotalStudentsByClassGroup();
            long endTime = System.currentTimeMillis();
            System.out.println("Cached Query Time: " + (endTime - startTime) + " ms");
        }, 0, 5, TimeUnit.SECONDS);

        // Data modifications every 10 seconds (simulate adding, deleting, updating)
        executorService.scheduleAtFixedRate(() -> {
            System.out.println("Simulating data modification...");
            studentRepository.addRandomStudent(); // Simulate adding
            studentRepository.deleteRandomStudent(); // Simulate deleting
            studentRepository.updateRandomStudent(); // Simulate updating
        }, 0, 10, TimeUnit.SECONDS);
    }
}

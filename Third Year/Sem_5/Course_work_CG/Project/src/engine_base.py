from mainwindow import MainWindow
from renderwidget import RenderWidget

from PyQt5.QtCore import QTimer, QTime
from PyQt5.QtWidgets import QApplication

from geometry import Vec3D
from camera import Camera

import sys


class ZBuffer:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        self.array = [float('+inf')] * width * height

    def get_z(self, x, y):
        if 0 <= x < self.width and 0 <= y < self.height:
            return self.array[x + y * self.width]

        return float('+inf')

    def set_z(self, x, y, value):
        if 0 <= x < self.width and 0 <= y < self.height:
            self.array[x + y * self.width] = value

    def update(self):
        self.array = [float('+inf')] * self.width * self.height


class EngineBase:
    def __init__(self):
        self.width = 675
        self.height = 675
        self.depth = 255

        self.app = QApplication(sys.argv)
        self.window = MainWindow(self)
        self.renderWidget = RenderWidget(self.width, self.height, self.window)

        self.timer = QTimer()
        self.timer.timeout.connect(self.image_render)
        self.timer.start(30)

        self.time = QTime()
        self.time.start()

        self.camera = Camera()

        self.z_buffer = ZBuffer(self.width, self.height)

    def image_render(self):
        self.z_buffer.update()
        self.update()
        self.renderWidget.update()

    def start(self):
        self.window.show()
        sys.exit(self.app.exec_())

    def set_pixel(self, x, y, r, g, b):
        if 0 <= x < self.width and 0 <= y < self.height:
            self.renderWidget.set_pixel(x, self.width - 1 - y, r, g, b)

    def fill(self, rgb):
        self.renderWidget.fill(rgb)

    def draw_line(self, start_point, end_point, red, green, blue):
        p0 = start_point.copy()
        p1 = end_point.copy()

        swapped = False
        if abs(p0.x - p1.x) < abs(p0.y - p1.y):
            p0.x, p0.y = p0.y, p0.x
            p1.x, p1.y = p1.y, p1.x
            swapped = True

        if p0.x > p1.x:
            p0, p1 = p1, p0

        dx = p1.x - p0.x
        dy = p1.y - p0.y
        d_error2 = abs(dy) * 2
        error2 = 0
        y = p0.y

        for x in range(int(p0.x), int(p1.x) + 1):
            if swapped:
                self.set_pixel(y, x, red, green, blue)
            else:
                self.set_pixel(x, y, red, green, blue)
            error2 += d_error2
            if error2 > dx:
                y += 1 if p1.y > p0.y else -1
                error2 -= dx * 2

    def draw_triangle(self, p0, p1, p2, red, green, blue):
        self.draw_line(p0, p1, red, green, blue)
        self.draw_line(p1, p2, red, green, blue)
        self.draw_line(p2, p0, red, green, blue)

    def render_shaded_triangle(self, qp0, qp1, qp2, qn0, qn1, qn2, q_red, q_green, q_blue):
        vertex0 = qp0.copy()
        vertex1 = qp1.copy()
        vertex2 = qp2.copy()

        normal0 = qn0.copy().normalize()
        normal1 = qn1.copy().normalize()
        normal2 = qn2.copy().normalize()

        vertex0.int()
        vertex1.int()
        vertex2.int()

        light_direction = Vec3D(1, 2, -0.5)
        light_direction.normalize()

        diffuse_intensity = 1

        intensity0 = min(max(0.05, Vec3D.dp(normal0, light_direction)) * diffuse_intensity, 1)
        intensity1 = min(max(0.05, Vec3D.dp(normal1, light_direction)) * diffuse_intensity, 1)
        intensity2 = min(max(0.05, Vec3D.dp(normal2, light_direction)) * diffuse_intensity, 1)

        if vertex0.y == vertex1.y and vertex0.y == vertex2.y:
            return
        if vertex0.y > vertex1.y:
            vertex0, vertex1 = vertex1, vertex0
            intensity0, intensity1 = intensity1, intensity0
        if vertex0.y > vertex2.y:
            vertex0, vertex2 = vertex2, vertex0
            intensity0, intensity2 = intensity2, intensity0
        if vertex1.y > vertex2.y:
            vertex1, vertex2 = vertex2, vertex1
            intensity1, intensity2 = intensity2, intensity1

        total_height = vertex2.y - vertex0.y

        for row in range(total_height):
            is_second_half = row > vertex1.y - vertex0.y or vertex1.y == vertex0.y
            segment_height = vertex2.y - vertex1.y if is_second_half else vertex1.y - vertex0.y
            alpha = row / total_height
            beta = (row - (vertex1.y - vertex0.y)) / segment_height if is_second_half else row / segment_height

            interpolated_a = Vec3D.add(vertex0, Vec3D.sub(vertex2, vertex0).mul(alpha))
            interpolated_a.int()

            interpolated_intensity_a = intensity0 + (intensity2 - intensity0) * alpha

            interpolated_b = Vec3D.add(vertex1, Vec3D.sub(vertex2, vertex1).mul(beta)) if is_second_half else \
                Vec3D.add(vertex0, Vec3D.sub(vertex1, vertex0).mul(beta))
            interpolated_b.int()

            interpolated_intensity_b = intensity1 + (intensity2 - intensity1) * beta if is_second_half else \
                intensity0 + (intensity1 - intensity0) * beta

            if interpolated_a.x > interpolated_b.x:
                interpolated_a, interpolated_b = interpolated_b, interpolated_a
                interpolated_intensity_a, interpolated_intensity_b = interpolated_intensity_b, interpolated_intensity_a

            for col in range(int(interpolated_a.x), int(interpolated_b.x) + 1):
                phi = 1 if interpolated_b.x == interpolated_a.x else (col - interpolated_a.x) / (
                            interpolated_b.x - interpolated_a.x)

                interpolated_p = Vec3D.add(interpolated_a, Vec3D.sub(interpolated_b, interpolated_a).mul(phi))
                interpolated_p.int()

                interpolated_intensity_p = interpolated_intensity_a + (
                            interpolated_intensity_b - interpolated_intensity_a) * phi

                if self.z_buffer.get_z(interpolated_p.x, vertex0.y + row) > interpolated_p.z:
                    self.z_buffer.set_z(interpolated_p.x, vertex0.y + row, interpolated_p.z)
                    self.set_pixel(interpolated_p.x, vertex0.y + row, interpolated_intensity_p * q_red,
                                   interpolated_intensity_p * q_green, interpolated_intensity_p * q_blue)

    def Fong_fill_triangle(self, renderPolygon0, renderPolygon1, renderPolygon2, renderNet0, renderNet1, renderNet2, red, green, blue):
        polygon0 = renderPolygon0.copy()
        polygon1 = renderPolygon1.copy()
        polygon2 = renderPolygon2.copy()

        net0 = renderNet0.copy().normalize()
        net1 = renderNet1.copy().normalize()
        net2 = renderNet2.copy().normalize()

        polygon0.int()
        polygon1.int()
        polygon2.int()

        if polygon0.y == polygon1.y and polygon0.y == polygon2.y:
            return
        if polygon0.y > polygon1.y:
            polygon0, polygon1 = polygon1, polygon0
            net0, net1 = net1, net0
        if polygon0.y > polygon2.y:
            polygon0, polygon2 = polygon2, polygon0
            net0, net2 = net2, net0
        if polygon1.y > polygon2.y:
            polygon1, polygon2 = polygon2, polygon1
            net1, net2 = net2, net1

        total_height = polygon2.y - polygon0.y

        for i in range(total_height):
            second_half = i > polygon1.y - polygon0.y or polygon1.y == polygon0.y
            segment_height = polygon2.y - polygon1.y if second_half else polygon1.y - polygon0.y
            alpha = i / total_height
            beta = (i - (polygon1.y - polygon0.y)) / segment_height if second_half else i / segment_height

            a = Vec3D.add(polygon0, Vec3D.sub(polygon2, polygon0).mul(alpha))
            a.int()

            u = Vec3D.dist(polygon0, a) / Vec3D.dist(polygon2, polygon0)
            n_a = Vec3D.add(Vec3D.mul_value(net0, 1 - u), Vec3D.mul_value(net2, u)).normalize()

            b = Vec3D.add(polygon1, Vec3D.sub(polygon2, polygon1).mul(beta)) if second_half else \
                Vec3D.add(polygon0, Vec3D.sub(polygon1, polygon0).mul(beta))
            b.int()

            w = Vec3D.dist(polygon1, b) / Vec3D.dist(polygon2, polygon1) if second_half else \
                Vec3D.dist(polygon0, b) / Vec3D.dist(polygon1, polygon0)

            n_b = Vec3D.add(Vec3D.mul_value(net1, 1 - w), Vec3D.mul_value(net2, w)).normalize() if second_half else \
                Vec3D.add(Vec3D.mul_value(net0, 1 - w), Vec3D.mul_value(net1, w)).normalize()

            if a.x > b.x:
                a, b = b, a
                n_a, n_b = n_b, n_a

            for j in range(a.x, b.x + 1):
                phi = 1 if b.x == a.x else (j - a.x) / (b.x - a.x)

                polygon = Vec3D.add(a, Vec3D.sub(b, a).mul(phi))
                polygon.int()

                n = n_a.copy()
                if not Vec3D.equal(a, b):
                    t = Vec3D.dist(a, polygon) / Vec3D.dist(b, a)
                    n = Vec3D.add(Vec3D.mul_value(n_a, 1 - t), Vec3D.mul_value(n_b, t)).normalize()

                light_position = Vec3D(1000, 2000, -500)
                light_direction = Vec3D.sub(light_position, polygon)
                light_direction.normalize()

                eye_direction = Vec3D.sub(self.camera.point, polygon)
                eye_direction.normalize()

                reflect_direction = Vec3D.sub(Vec3D.mul_value(n, 2 * Vec3D.dp(n, light_direction)), light_direction).mul(-1)
                reflect_direction.normalize()

                kd = 0.75
                ks = 0.25
                spect_alpha = 8

                ambient = 0.25
                diffuse = Vec3D.dp(n, light_direction)
                spect = Vec3D.dp(reflect_direction, eye_direction) ** spect_alpha
                intensity = ks * spect + kd * diffuse + kd * ambient

                if intensity > 1:
                    intensity = 1

                if self.z_buffer.get_z(polygon.x, polygon0.y + i) > polygon.z:
                    self.z_buffer.set_z(polygon.x, polygon0.y + i, polygon.z)
                    self.set_pixel(polygon.x, polygon0.y + i, intensity * red, intensity * green, intensity * blue)

    def create(self):
        # الخطأ الناتج عن هي الوظيفة ما عم يخليني شغل البرنامج!
        pass

    def update(self):
        # الخطأ الناتج عن هي الوظيفة ما عم يخليني شغل البرنامج!
        pass

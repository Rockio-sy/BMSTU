from geometry import *

class Camera:
    def __init__(self):
        self.point = Vec3D(2, 5, -2.5)

        self.forward = Vec3D(0, 0, 1)
        self.up = None
        self.right = None
        self.top = Vec3D(0, 1, 0)

        self.yaw = -36
        self.pitch = -35

        self.speed = 0.5
        self.d_yaw = 2
        self.d_pitch = 2

        self.update()

    def button_w(self):
        forward = self.forward.copy()
        self.point = Vec3D.add(self.point, forward.mul(self.speed))

    def button_s(self):
        forward = self.forward.copy()
        self.point = Vec3D.sub(self.point, forward.mul(self.speed))

    def button_a(self):
        right = self.right.copy()
        self.point = Vec3D.sub(self.point, right.mul(self.speed))

    def button_d(self):
        right = self.right.copy()
        self.point = Vec3D.add(self.point, right.mul(self.speed))

    def up_arrow(self):
        self.pitch += self.d_pitch
        self.update()

    def down_arrow(self):
        self.pitch -= self.d_pitch
        self.update()

    def left_arrow(self):
        self.yaw += self.d_yaw
        self.update()

    def right_arrow(self):
        self.yaw -= self.d_yaw
        self.update()

    def update(self):
        r_yaw = self.yaw / 180 * math.pi
        r_pitch = self.pitch / 180 * math.pi

        self.forward.x = math.sin(r_yaw) * math.cos(r_pitch)
        self.forward.y = math.sin(r_pitch)
        self.forward.z = math.cos(r_yaw) * math.cos(r_pitch)
        self.forward.normalize()

        self.right = Vec3D.cp(self.forward, self.top)
        self.right.normalize()
        self.up = Vec3D.cp(self.right, self.forward)
        self.up.normalize()

    def get_mat_view(self):
        mat_camera = MatPointAt(self.point, self.forward, self.up, self.right)
        return mat_camera.quick_inverse()

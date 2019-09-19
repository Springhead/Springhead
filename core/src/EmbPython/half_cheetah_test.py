import time
from Spr import *
import numpy as np

app = EPApp()
#input()
app.InitInNewThread()
time.sleep(1)
fwSdk = FWSdk()
fwSdk = app.GetSdk()
phSdk = PHSdk()
phSdk = fwSdk.GetPHSdk()
fwScene = fwSdk.GetScene()
phScene = PHScene()
phScene = fwScene.GetPHScene()
gravityVec = phScene.GetGravity()
zeroGravity = Vec3d(0, 0, 0)
# phScene.SetGravity(zeroGravity)
phScene.SetGravity(Vec3d(0, -9.8, 0))

solid_desc = PHSolidDesc()
box_desc = CDBoxDesc()
box = CDBox()
cupsule_desc = CDCapsuleDesc()
cupsule = CDCapsule()
fix_joint_desc = PHFixJointDesc()
fix_joint = PHFixJoint()
hinge_joint_desc = PHHingeJointDesc()
hinge_joint = PHHingeJoint()
limit_desc = PH1DJointLimitDesc()
# 回転用変数
qua_no_rot = Quaterniond()
qua_z_90 = Quaterniond().Rot(np.pi / 2, 'z')
qua_y_90 = Quaterniond().Rot(np.pi / 2, 'y')
qua_x_90 = Quaterniond().Rot(-np.pi / 2, 'x')
qua_x_m90 = Quaterniond().Rot(np.pi / 2, 'x')
qua_x_180 = Quaterniond().Rot(-np.pi, 'x')
qua_x_45 = Quaterniond().Rot(-np.pi / 4, 'x')
qua_y_45 = Quaterniond().Rot(np.pi / 4, 'y')
# 接触モード用変数
MODE_NONE = 0
MODE_LCP = 1
MODE_PENALTY = 2


class ModelWrapper:
    def __init__(self, model, init_pos, init_qua, model_so):
        self.model = model
        self.model_so = model_so
        self.init_pos = init_pos
        self.init_qua = init_qua

    def set_dynamics(self, flag):
        self.model.SetDynamical(flag)

    def initialize_position(self):
        self.model.SetFramePosition(self.init_pos)
        self.model.SetOrientation(self.init_qua)

    def recreate(self, ph_scene):
        self.model = ph_scene.CreateSolid(solid_desc)
        self.model.AddShape(self.model_so)
        self.model.SetOrientation(self.init_qua)
        self.model.SetFramePosition(self.init_pos)


class JointWrapper:
    def __init__(self, joint, init_pos, init_vel, socket, plug, pose_socket, pose_plug):
        self.joint = joint
        self.init_pos = init_pos
        self.init_vel = init_vel
        self.socket = socket
        self.plug = plug
        self.pose_socket = pose_socket
        self.pose_plug = pose_plug
        self.range_limit = Vec2d(1, -1)

    def recreate(self, ph_scene, joint_obj=hinge_joint, joint_desc=hinge_joint_desc):
        joint_desc.poseSocket = self.pose_socket
        joint_desc.posePlug = self.pose_plug
        self.joint = ph_scene.CreateJoint(self.socket.model, self.plug.model, joint_obj.GetIfInfoStatic(), joint_desc)

    def set_initial_position(self):
        self.joint.SetTargetPosition(self.init_pos)

    def set_initial_velocity(self):
        self.joint.SetTargetVelocity(self.init_vel)

    def set_target_pos(self, pos):
        self.joint.SetTargetPosition(pos)

    def set_target_vel(self, vel):
        self.joint.SetTargetVelocity(vel)

    def set_spring(self, spring):
        self.joint.SetSpring(spring)

    def set_damper(self, damper):
        self.joint.SetDamper(damper)

    def set_range_limit(self, range_min, range_max):
        self.range_limit.x = self.init_pos + range_min
        self.range_limit.y = self.init_pos + range_max

    def activate_range_limit(self):
        limit_desc.range = Vec2d(self.range_limit)
        self.joint.CreateLimit(limit_desc)


def app_initialization():
    return


def main():
    # half Cheetahを作ってみる
    # 床
    floor_pos_hight = -0.97
    box_desc.boxsize = Vec3d(1000, 1, 1000)
    floor_so = phSdk.CreateShape(box.GetIfInfoStatic(), box_desc)
    floor = phScene.CreateSolid(solid_desc)
    fwScene.SetSolidMaterial(23, floor)
    floor.AddShape(floor_so)
    floor.SetFramePosition(Vec3d(0, floor_pos_hight, 0))
    floor.SetDynamical(False)
    # torso
    torso_length = 0.454 * 2
    cupsule_radius = 0.046
    cupsule_desc.length = torso_length
    cupsule_desc.radius = cupsule_radius
    torso_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    torso = phScene.CreateSolid(solid_desc)
    torso.AddShape(torso_so)
    qua_torso = qua_y_90
    torso.SetOrientation(qua_torso)  # 物体座標での回転
    torso_wrapper = ModelWrapper(torso, Vec3d(0, 0, 0), qua_torso, torso_so)
    # head
    head_length = 0.3
    cupsule_desc.length = head_length
    cupsule_desc.radius = cupsule_radius
    head_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    head = phScene.CreateSolid(solid_desc)
    head.AddShape(head_so)
    init_head_pos = Vec3d(torso_length / 2 + cupsule_radius * 2, head_length / 2 / np.sqrt(2), 0)
    head.SetFramePosition(init_head_pos)
    qua_head = qua_y_90 * qua_x_45
    head.SetOrientation(qua_head)
    head_wrapper = ModelWrapper(head, init_head_pos, qua_head, head_so)
    # torso <-> head fix joint
    pose_socket_1 = Posed(Vec3d(0, 0, -head_length / 2), Quaterniond())
    pose_plug_1 = Posed(Vec3d(0, 0, torso_length / 2), Quaterniond(qua_x_45))
    fix_joint_desc.poseSocket = pose_socket_1  # 物体依存の座標系
    fix_joint_desc.posePlug = pose_plug_1  # 物体依存の座標系
    torso_head_joint = phScene.CreateJoint(head, torso, fix_joint.GetIfInfoStatic(), fix_joint_desc)
    torso_head_joint_wrapper = JointWrapper(torso_head_joint, 0, 0, head_wrapper, torso_wrapper, pose_socket_1, pose_plug_1)
    # back_thigh
    back_thigh_length = 0.145 * 2
    cupsule_desc.length = back_thigh_length
    cupsule_desc.radius = cupsule_radius
    back_thigh_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    back_thigh = phScene.CreateSolid(solid_desc)
    back_thigh.AddShape(back_thigh_so)
    qua_back_thigh = qua_y_90 * qua_x_90
    back_thigh.SetOrientation(qua_back_thigh)
    init_back_thigh_pos = Vec3d(-torso_length / 2, -back_thigh_length / 2, 0)
    back_thigh.SetFramePosition(init_back_thigh_pos)
    back_thigh_wrapper = ModelWrapper(back_thigh, init_back_thigh_pos, qua_back_thigh, back_thigh_so)
    # torso <-> back_thigh hinge joint
    pose_socket_2 = Posed(Vec3d(0, 0, back_thigh_length / 2), qua_y_90)
    pose_plug_2 = Posed(Vec3d(0, 0, -torso_length / 2), qua_x_90 * qua_y_90)
    hinge_joint_desc.poseSocket = pose_socket_2  # 回転方向ok
    hinge_joint_desc.posePlug = pose_plug_2
    torso_back_thigh_joint = phScene.CreateJoint(back_thigh, torso, hinge_joint.GetIfInfoStatic(), hinge_joint_desc)
    torso_back_thigh_joint_wrapper = JointWrapper(torso_back_thigh_joint, 0, 0, back_thigh_wrapper, torso_wrapper, pose_socket_2, pose_plug_2)
    # front_thigh
    front_thigh_length = 0.145 * 2
    cupsule_desc.length = front_thigh_length
    cupsule_desc.radius = cupsule_radius
    front_thigh_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    front_thigh = phScene.CreateSolid(solid_desc)
    front_thigh.AddShape(front_thigh_so)
    qua_front_thigh = qua_y_90 * qua_x_90
    front_thigh.SetOrientation(qua_front_thigh)
    init_front_thigh_pos = Vec3d(torso_length / 2, -front_thigh_length / 2, 0)
    front_thigh.SetFramePosition(init_front_thigh_pos)
    front_thigh_wrapper = ModelWrapper(front_thigh, init_front_thigh_pos, qua_front_thigh, front_thigh_so)
    # torso <-> front_thigh hinge joint
    pose_socket_3 = Posed(Vec3d(0, 0, front_thigh_length / 2), qua_y_90)
    pose_plug_3 = Posed(Vec3d(0, 0, torso_length / 2), qua_x_90 * qua_y_90)
    hinge_joint_desc.poseSocket = pose_socket_3  # 回転方向ok
    hinge_joint_desc.posePlug = pose_plug_3
    torso_front_thigh_joint = phScene.CreateJoint(front_thigh, torso, hinge_joint.GetIfInfoStatic(), hinge_joint_desc)
    torso_front_thigh_joint_wrapper = JointWrapper(torso_front_thigh_joint, 0, 0, front_thigh_wrapper, torso_wrapper, pose_socket_3, pose_plug_3)
    # back_shin
    back_shin_length = 0.15 * 2
    cupsule_desc.length = back_shin_length
    cupsule_desc.radius = cupsule_radius
    back_shin_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    back_shin = phScene.CreateSolid(solid_desc)
    back_shin.AddShape(back_shin_so)
    qua_back_shin = qua_y_90
    back_shin.SetOrientation(qua_back_shin)
    init_back_shin_pos = Vec3d(-back_shin_length / 2 - cupsule_radius / 2, -back_thigh_length / 2, 0) + back_thigh.GetFramePosition()
    back_shin.SetFramePosition(init_back_shin_pos)
    back_shin_wrapper = ModelWrapper(back_shin, init_back_shin_pos, qua_back_shin, back_shin_so)
    # back_thigh <-> back_shin hinge joint
    pose_socket_4 = Posed(Vec3d(0, 0, back_shin_length / 2), qua_y_90)
    pose_plug_4 = Posed(Vec3d(0, 0, -back_thigh_length / 2), qua_x_90 * qua_y_90)
    hinge_joint_desc.poseSocket = pose_socket_4
    hinge_joint_desc.posePlug = pose_plug_4
    back_thigh_back_shin_joint = phScene.CreateJoint(back_shin, back_thigh, hinge_joint.GetIfInfoStatic(), hinge_joint_desc)
    back_thigh_back_shin_joint_wrapper = JointWrapper(back_thigh_back_shin_joint, -np.pi, 0, back_shin_wrapper, back_thigh_wrapper, pose_socket_4, pose_plug_4)
    # front_shin
    front_shin_length = 0.106 * 2
    cupsule_desc.length = front_shin_length
    cupsule_desc.radius = cupsule_radius
    front_shin_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    front_shin = phScene.CreateSolid(solid_desc)
    front_shin.AddShape(front_shin_so)
    qua_front_shin = qua_y_90
    front_shin.SetOrientation(qua_front_shin)
    init_front_shin_pos = Vec3d(front_shin_length / 2 + cupsule_radius / 2, -front_thigh_length / 2, 0) + front_thigh.GetFramePosition()
    front_shin.SetFramePosition(init_front_shin_pos)
    front_shin_wrapper = ModelWrapper(front_shin, init_front_shin_pos, qua_front_shin, front_shin_so)
    # front_thigh <-> front_shin hinge joint
    pose_socket_5 = Posed(Vec3d(0, 0, -front_shin_length / 2), qua_y_90)
    pose_plug_5 = Posed(Vec3d(0, 0, -front_thigh_length / 2), qua_x_90 * qua_y_90)
    hinge_joint_desc.poseSocket = pose_socket_5
    hinge_joint_desc.posePlug = pose_plug_5
    front_thigh_front_shin_joint = phScene.CreateJoint(front_shin, front_thigh, hinge_joint.GetIfInfoStatic(), hinge_joint_desc)
    front_thigh_front_shin_joint_wrapper = JointWrapper(front_thigh_front_shin_joint, -np.pi, 0,
                                                        front_shin_wrapper, front_thigh_wrapper, pose_socket_5, pose_plug_5)
    # front_thigh_front_shin_joint.SetTargetVelocity(10)
    # front_thigh_front_shin_joint.SetDamper(100)
    # back_foot
    back_foot_length = 0.094 * 2
    cupsule_desc.length = back_foot_length
    cupsule_desc.radius = cupsule_radius
    back_foot_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    back_foot = phScene.CreateSolid(solid_desc)
    back_foot.AddShape(back_foot_so)
    qua_back_foot = qua_y_90 * qua_x_90
    back_foot.SetOrientation(qua_back_foot)
    init_back_foot_pos = Vec3d(-back_shin_length / 2, -back_foot_length / 2, 0) + back_shin.GetFramePosition()
    back_foot.SetFramePosition(init_back_foot_pos)
    back_foot_wrapper = ModelWrapper(back_foot, init_back_foot_pos, qua_back_foot, back_foot_so)
    # back_shin <-> back_foot hinge joint
    pose_socket_6 = Posed(Vec3d(0, 0, -back_shin_length / 2), qua_y_90)
    pose_plug_6 = Posed(Vec3d(0, 0, back_foot_length / 2), qua_x_90 * qua_y_90)
    hinge_joint_desc.poseSocket = pose_socket_6
    hinge_joint_desc.posePlug = pose_plug_6
    back_shin_back_foot_joint = phScene.CreateJoint(back_shin, back_foot, hinge_joint.GetIfInfoStatic(),
                                                    hinge_joint_desc)
    back_shin_back_foot_joint_wrapper = JointWrapper(back_shin_back_foot_joint, -np.pi, 0,
                                                     back_shin_wrapper, back_foot_wrapper, pose_socket_6, pose_plug_6)
    # front_foot
    front_foot_length = 0.094 * 2
    cupsule_desc.length = front_foot_length
    cupsule_desc.radius = cupsule_radius
    front_foot_so = phSdk.CreateShape(cupsule.GetIfInfoStatic(), cupsule_desc)
    front_foot = phScene.CreateSolid(solid_desc)
    front_foot.AddShape(front_foot_so)
    qua_front_foot = qua_y_90
    front_foot.SetOrientation(qua_front_foot)
    init_front_foot_pos = Vec3d(front_shin_length / 2 + front_foot_length / 2, 0, 0) + front_shin.GetFramePosition()
    front_foot.SetFramePosition(init_front_foot_pos)
    front_foot_wrapper = ModelWrapper(front_foot, init_front_foot_pos, qua_front_foot, front_foot_so)
    # front_shin <-> front_foot hinge joint
    pose_socket_7 = Posed(Vec3d(0, 0, front_shin_length / 2), qua_y_90)
    pose_plug_7 = Posed(Vec3d(0, 0, -front_foot_length / 2), qua_y_90)
    hinge_joint_desc.poseSocket = pose_socket_7
    hinge_joint_desc.posePlug = pose_plug_7
    front_shin_front_foot_joint = phScene.CreateJoint(front_shin, front_foot, hinge_joint.GetIfInfoStatic(),
                                                      hinge_joint_desc)
    front_shin_front_foot_joint_wrapper = JointWrapper(front_shin_front_foot_joint, np.pi / 3, 0,
                                                       front_shin_wrapper, front_foot_wrapper, pose_socket_7, pose_plug_7)
    # modelのリスト化
    model_wrappers = [torso_wrapper, head_wrapper, back_thigh_wrapper, front_thigh_wrapper,
              back_shin_wrapper, front_shin_wrapper, back_foot_wrapper, front_foot_wrapper]
    # jointのリスト化
    hinge_joint_wrappers = [torso_back_thigh_joint_wrapper, torso_front_thigh_joint_wrapper,
              back_thigh_back_shin_joint_wrapper, front_thigh_front_shin_joint_wrapper,
              back_shin_back_foot_joint_wrapper, front_shin_front_foot_joint_wrapper]
    # 衝突判定の削除
    set_contact_mode_all(model_wrappers, phScene, MODE_NONE)
    # 駆動範囲の設定
    for j in hinge_joint_wrappers:
        j.set_range_limit(-np.pi /4, np.pi / 4)
    torso_back_thigh_joint_wrapper.set_range_limit(-np.pi / 4, 0)
    torso_front_thigh_joint_wrapper.set_range_limit(-np.pi / 4, np.pi / 10)

    fwScene.EnableRenderGrid(False, True, False)

    win = FWWin()
    win = app.GetCurrentWin()
    win.GetTrackball().SetPosition(Vec3f(0, 1, 1))
    win.GetTrackball().SetTarget(Vec3f(0, 0, 0))

    return app, win, model_wrappers, hinge_joint_wrappers, torso_head_joint_wrapper


def do_one_simulation(app, win, model_wrappers,
                      hinge_joint_wrappers, torso_head_joint_wrapper,
                      policy, is_display=True):
    phScene.Step()
    initialize_model(model_wrappers, hinge_joint_wrappers, torso_head_joint_wrapper)
    old_state = np.zeros((policy.state_dim, 1))
    state = get_state(model_wrappers[0], hinge_joint_wrappers, old_state)
    old_state = state
    reward = 0
    for i in range(2000):
        state = get_state(model_wrappers[0], hinge_joint_wrappers, old_state)
        action = policy.get_action(state)
        set_action(hinge_joint_wrappers, action)
        phScene.Step()
        reward += state[0] - old_state[0]
        if is_display:
            app.PostRedisplay()
            win.GetTrackball().SetTarget(model_wrappers[0].model.GetFramePosition())
        # force z angle
        force_z_angle_all(model_wrappers)
        old_state = state
        if is_display:
            time.sleep(0.00001)
    return reward


def force_z_angle_all(cd_list):
    for model in cd_list:
        if isinstance(model, ModelWrapper):
            nowvec = model.model.GetFramePosition()
            model.model.SetFramePosition(Vec3d(nowvec.x, nowvec.y, 0))
        else:
            nowvec = model.GetFramePosition()
            model.SetFramePosition(Vec3d(nowvec.x, nowvec.y, 0))


def set_contact_mode_all(cd_list, phScene, mode):
    cd_size = len(cd_list)
    for i in range(cd_size):
        for j in range(i + 1, cd_size):
            if isinstance(cd_list[i], ModelWrapper):
                mem_i = cd_list[i].model
            else:
                mem_i = cd_list[i]
            if isinstance(cd_list[j], ModelWrapper):
                mem_j = cd_list[j].model
            else:
                mem_j = cd_list[j]
            phScene.SetContactMode(mem_i, mem_j, mode)


def initialize_model(model_wrappers, joint_wrappers, tor_head_joint):
    for m in model_wrappers:
        phScene.DelChildObject(m.model)
    for j in joint_wrappers:
        phScene.DelChildObject(j.joint)
    phScene.DelChildObject(tor_head_joint.joint)
    time.sleep(0.1)
    for model in model_wrappers:
        model.recreate(phScene)
    set_contact_mode_all(model_wrappers, phScene, MODE_NONE)
    for j in joint_wrappers:
        j.recreate(phScene)
        j.activate_range_limit()
        j.set_spring(100)
        j.set_initial_position()
    tor_head_joint.recreate(phScene, fix_joint, fix_joint_desc)


def calc_abs_euler_angle_x_y(qua):
    x = qua.x
    y = qua.y
    z = qua.z
    w = qua.w
    cos = 1 - 2 * y ** 2 - 2 * z ** 2
    return np.arccos(cos)


def calc_euler_angle_x_y(qua):
    x = qua.x
    y = qua.y
    z = qua.z
    w = qua.w
    cos = 1 - 2 * y ** 2 - 2 * z ** 2
    sin = 2 * x * y + 2 * w * z
    if sin > 0:
        return np.arccos(cos)
    else:
        return -np.arccos(cos)


class Policy:
    def __init__(self):
        self.state_dim = 18
        self.action_dim = 6
        self.theta = np.zeros([self.action_dim, self.state_dim])

    def get_action(self, state):
        action = np.dot(self.theta, state)
        return action

    def set_theta(self, vector):
        self.theta = np.reshape(vector, (self.action_dim, self.state_dim))

    def get_theta_dim(self):
        return self.action_dim * self.state_dim


def get_state(torso_wrapper, joint_wrapper, old_state):
    dim = 18
    half_dim = int(dim / 2)
    state = np.zeros((dim, 1))
    state[0, 0] = torso_wrapper.model.GetFramePosition().x
    state[1, 0] = torso_wrapper.model.GetFramePosition().y
    state[2, 0] = calc_euler_angle_x_y(torso_wrapper.model.GetOrientation())
    for idx, j in enumerate(joint_wrapper):
        state[idx + 3, 0] = j.joint.GetPosition()
    for idx in range(half_dim):
        state[idx + half_dim, 0] = state[idx, 0] - old_state[idx, 0]
    return state


def set_action(joint_wrapper, action):
    for idx, j in enumerate(joint_wrapper):
        j.set_target_pos(action[idx, 0] + j.init_pos)


def model_test():
    app, win, model_wrappers, hinge_joint_wrappers, torso_head_joint_wrapper = main()
    policy = Policy()
    best_vector = np.loadtxt("best_vector.txt")
    policy.set_theta(best_vector)
    reward = do_one_simulation(app, win, model_wrappers,
                                       hinge_joint_wrappers, torso_head_joint_wrapper,
                                       policy)


if __name__ == "__main__":
    # model_test()
    # exit()
    np.random.seed(12312)
    app, win, model_wrappers, hinge_joint_wrappers, torso_head_joint_wrapper = main()
    policy = Policy()

    d = policy.get_theta_dim()
    lam = 10
    stop_condition = lambda g: g > 100

    evals = np.empty(lam)
    best_eval = 0
    best_vector = []

    g = 0
    while not stop_condition(g):
        print("g: " + str(g))
        samples = np.random.rand(d, lam) * 2.0 - 1.0
        for idx in range(lam):
            policy.set_theta(samples[:, idx])
            reward = do_one_simulation(app, win, model_wrappers,
                                       hinge_joint_wrappers, torso_head_joint_wrapper,
                                       policy, is_display=True)
            evals[idx] = -reward
            if evals[idx] < best_eval:
                best_eval = evals[idx]
                best_vector = samples[:, idx]
            print(reward)
        if best_eval < -50:
            break
        g += 1
    np.savetxt("best_vector.txt", best_vector)
    policy.set_theta(best_vector)
    reward = do_one_simulation(app, win, model_wrappers,
                                       hinge_joint_wrappers, torso_head_joint_wrapper,
                                       policy)

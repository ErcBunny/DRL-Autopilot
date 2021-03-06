# Paper Reading

[TOC]

---

## Taxonomy & Comments

* Surveys
  *  [14 Review of drone control algorithms (OJAPPS).pdf](14 Review of drone control algorithms (OJAPPS).pdf) 
  *  [20 UAV control survey (EAI).pdf](20 UAV control survey (EAI).pdf) 
     *  PID: essentially some kind of implicit planning and tracking, tracking performance isn't good, and the long pipeline of cascaded control is making this worse
     *  basic MPC: Optimal, flexible, good tracking; because there is always a feasible trajectory given, in this way, we have more control during the step process
  *  [21 Drone DRL survey (electronics).pdf](21 Drone DRL survey (electronics).pdf)

* For general purpose intelligent flight
  * Percept & Plan & Track pipeline
    * perception
    * generation of a trajectory
      *  [21 DRL traj Planning for drone racing (IROS).pdf](21 DRL traj Planning for drone racing (IROS).pdf) 
      *  [21 Time-opt planning for drone racing (SR).pdf](21 Time-opt planning for drone racing (SR).pdf) 
      *  [21 Whole-Body Planning (ICRA).pdf](21 Whole-Body Planning (ICRA).pdf) 
    * tracking controller
      * MPC family
        *  [12 Learning based MPC (ICRA).pdf](12 Learning based MPC (ICRA).pdf) 
        *  [21 Data driven MPC (RAL).pdf](21 Data driven MPC (RAL).pdf) 
        *  [21 Adaptive NLMPC (Arxiv).pdf](21 Adaptive NLMPC (Arxiv).pdf) (maybe the best MPC so far, with disturbance rejection, but not unified)
      * RL E2E controller
        *  [17 Control of a Quadrotor with RL (RAL).pdf](17 Control of a Quadrotor with RL (RAL).pdf) 
        *  [19 l2fly RL (ACM).pdf](19 l2fly RL (ACM).pdf) 
        *  [20 RL control of tilt quads (DSCC).pdf](20 RL control of tilt quads (DSCC).pdf)
        *  [21 Low level RL Control (Sensors).pdf](21 Low level RL Control (Sensors).pdf) 
      * Better tracking with responsive inner loop
        *  [17 Traj tracking LQR body-rate control (RAL).pdf](17 Traj tracking LQR body-rate control (RAL).pdf) 
        *   [19 DRL gazebo attitude controller.pdf](19 DRL gazebo attitude controller.pdf) 
  * Unified
    *  [16 Unified planning and tracking NMPC(ICRA).pdf](16 Unified planning and tracking NMPC(ICRA).pdf) 
    *  [18 PAMPC (IROS).pdf](18 PAMPC (IROS).pdf) 
    *  [20 Deep Drone Acrobatics (RSS).pdf](20 Deep Drone Acrobatics (RSS).pdf) 
    *  [21 MPCC drone racing (Arxiv).pdf](21 MPCC drone racing (Arxiv).pdf) 
    *  [21 RAPTOR (TOR).pdf](21 RAPTOR (TOR).pdf) (perception aware)
    *  [21 Learning highspeed flight (SR).pdf](21 Learning highspeed flight (SR).pdf) (a typical example of E2E senor-motor agent)
      * perception is involved
      * significantly faster than traditional pipeline in terms of latency and calculation speed
      * reduces processing latency and increases robustness to noisy and incomplete perception
      * less compounding of errors through the pipeline
* For completing a specific task
  *  [18 Learning Target Following (IJCAI).pdf](18 Learning Target Following (IJCAI).pdf) 
  *  [19 Narrow gap crossing with E2E network (IROS).pdf](19 Narrow gap crossing with E2E network (IROS).pdf) (unified plan & control with RL tuning with a goal specified)
  *  [21 Narrow gap crossing DRL (TNNLS).pdf](21 Narrow gap crossing DRL (TNNLS).pdf) 
* Relative work
  *  [19 Sim2real of low level DL controllers (IROS).pdf](19 Sim2real of low level DL controllers (IROS).pdf) 
  *  [21 Identify hybrid aerodynamic model (RSS).pdf](21 Identify hybrid aerodynamic model (RSS).pdf) 
  *   [21 smooth learned output.pdf](21 smooth learned output.pdf) 

## 17 Control of a Quadrotor with RL (RAL)

1. System overview and capability

   * diagram

     <img src="./Screen Shot 2021-09-30 at 1.34.14 PM.png" alt="Screen Shot 2021-09-30 at 1.34.14 PM" style="zoom:50%;" />

   * capability and performance

     * recover from a random initial state and go to a desired state

       > During policy optimization, we train it to go to the origin of the inertial frame, and the model is deployed to a real quadcopter.
       >
       > We added a simple Proportional and Derivative (PD) controller for attitude with low gains along with our learning policy. The sum of the outputs of the two controllers are used as a command.
       >
       > The PD controller is used to stabilize the learning process but it does not aid the final controller.

     * track way points

       > During operation, we input the state subtracted by the waypoint location to the policy. This way we do not have to train waypoint tracking explicitly

       <img src="./Screen Shot 2021-09-30 at 1.55.26 PM.png" alt="Screen Shot 2021-09-30 at 1.55.26 PM" style="zoom:50%;" />

     * tracking performance suffers from rotor dynamics, aerodynamics, battery level and external disturbances

2. Network structure

   * two networks used for training, namely a value network and a policy network

     * output of policy network is rotor thrust
     * value network outputs the value (How good a state is)

   * Both networks have the state as an input, input vector (18) consists of

     * rotation mat (9) instead of quaternion
     * position (3)
     * angular and linear velocity (9)

   * diagram

     <img src="./Screen Shot 2021-09-30 at 1.32.44 PM.png" alt="Screen Shot 2021-09-30 at 1.32.44 PM" style="zoom:50%;" />

3. Optimization/training

   * Each learning iteration consists of
     * exploration
     * value network update
     * policy update

   * Exploration strategy

     * initial trajectories

     * junction trajectories are off-policy generated with an additive Gaussian noise with covariance ??

     * branch trajectories are on-policy trajectories starting from some state along the junction trajectories

       <img src="./Screen Shot 2021-09-30 at 2.19.25 PM.png" alt="Screen Shot 2021-09-30 at 2.19.25 PM" style="zoom:50%;" />

       > During exploration, we mostly follow the current policy(decision) to evaluate the value of the visited states. 
       >
       > If we only follow the current policy, it is unlikely to find a better policy, so noise is added and junctions are generated.
       >
       > After getting data from exploration, the value function is updated.
       >
       > Every time we add noise, we get a pair of trajectories. The two paths are evaluated according to 1.the cost it observed on the traj. and 2.the value of the state it eneded up. Then the better policy (traj) is selected (policy update). 

   * Value function training

     * value function: 
       $$
       v_i=\sum_{t=i}^{T-1}\gamma^{t-i}r_t^p+\gamma^{T-i}V(s_T|\eta)
       $$
       where

       * $\eta$: parameters
       * $T$: length of the trajectory

     * trained using Monte-Carlo samples that are obtained from on-policy trajectories

   * Policy opt (TODO)

   * Training in practice (TODO)

     * **[www.raisim.com](http://www.raisim.com/)**

4. Future work

   * introducing more accurate model of the system into the simulation
   * train RNN which can adapt to errors in modeling automatically
   * transfer learning on the real system can further improve the performance of the policy by capturing totally unknown dynamic aspects
     * ???????????????????????????????????????, ???????????????: ????????????, ???????????????????????????????????????????????????, ?????????????????????????????????. ?????????????????????????????????????????????????????????????????????????????????.

5. my comments

   * amazing idea
   * training process should be studied in detail in the future, also look at RNN and transfer learning

## 19 Narrow gap crossing with E2E network (IROS)

1. System overview and capability

   * diagram

     <img src="./Screen Shot 2021-09-30 at 9.06.48 PM.png" alt="Screen Shot 2021-09-30 at 9.06.48 PM" style="zoom:50%;" />

     * the network outputs the desired roll pitch and throttle which is then executed by FC
     * TYPO: "Leaning" should be "Learning"

   * capability and performance

     * given a working planner and a controller, using the RL-tuned network to imitate the function can achieve better result in some aspects but also introduces some undisired features
     * trained a network to do planning and tracking in an E2E way, RL acts as a opt tool that minimizes the average thrust and average angular velocity
     * the proposed method (network) imitates the traditional pipeline well in pos control, but isn't as smooth in vel and attitude control
     * The feed-forward process of our planning and end-to-end network cost about 3 ??? 5ms and 6 ??? 7ms, respectively

2. network structure

   * cascaded network structure: planning network -> controller network

   * planning network

     <img src="./Screen Shot 2021-09-30 at 9.39.05 PM.png" alt="Screen Shot 2021-09-30 at 9.39.05 PM" style="zoom:50%;" />

     > M. W. Mueller, M. Hehn, and R. D???Andrea, ???**A computationally efficient motion primitive for quadrocopter trajectory generation**,??? *IEEE Transactions on Robotics*, vol. 31, no. 6, pp. 1294???1310, 2015.

   * controller network

     <img src="./Screen Shot 2021-09-30 at 9.39.25 PM.png" alt="Screen Shot 2021-09-30 at 9.39.25 PM" style="zoom:50%;" />

     > D. Mellinger and V. Kumar, ???**Minimum snap trajectory generation and control for quadrotors**,??? in *Robotics and Automation (ICRA), 2011 IEEE International Conference on*. IEEE, 2011, pp. 2520???2525.
     >
     > T.Lee,M.Leok,andN.H.McClamroch,???**Geometrictrackingcontrol of a quadrotor uav on se (3)**,??? in *49th IEEE conference on decision and control (CDC)*. IEEE, 2010, pp. 5420???5425.

     * why output throttle and the desired roll and pitch? maybe related to N3 FC API

3. training

   * planner & controller: supervised by a pre-built planner and controller
   * RL
     * airsim
     * reward functions
     * OPENAI-Trust Region Policy Optimization

4. future work

   * investigating the possibility of a full end-to-end approach in UAV autonomous navigation and control
   * **However, the potential of an end-to-end approach lies in improving the perception, mapping, and estimation of model-based methods**

5. my comments

   * E2E consumes more time than traditional pipeline and introduces undesired features
   * it seems that the major contribution is done by the RL process, cauz it is essentially optimization inside
   * maybe it is easier to tune params of the imitated network than designing a parameterized planning method and tuning it for a better result, and maybe better patterns can emerge from the network

6. comments from **Flying Through a Narrow Gap Using End-to-end Deep Reinforcement Learning Augmented with Curriculum Learning and Sim2Real**

   * the first known study that implemented gap traversing using reinforcement learning
   * the initial trajectory being cloned is still obtained from the optimal control framework with excessive priors
   * imitation learning may still end up with local optimal solutions that are similar to demonstrations without sufficient exploration
   * not detached from optimal control that requires excessive priors

## 21 Narrow gap crossing DRL (TNNLS)

1. System overview and capability

   * diagram

     <img src="./Screen Shot 2021-09-30 at 10.39.07 PM.png" alt="Screen Shot 2021-09-30 at 10.39.07 PM" style="zoom:50%;" />

     <img src="./Screen Shot 2021-09-30 at 10.51.21 PM.png" alt="Screen Shot 2021-09-30 at 10.51.21 PM" style="zoom:50%;" />

     * action is linear and angular acceleration, position contains both translational and rotational part
     * sim2real calculates the position of next step and sends the position cmd to PX4 offboard mode

   * capability and performance

     * fly through a narrow tilted (20 deg) gap with a success rate related to the dimension of the gap
     * narrow gap traverse is purely done with reinforcement learning + curriculum learning, curriculum learning is incorporated to deal with the sparse reward problem
     * a sim2real method to ehance generalization of the policy without utilizing real-world data

2. Network, action sampler & sim2real

   * policy network & sampler

     <img src="./Screen Shot 2021-09-30 at 11.45.32 PM.png" alt="Screen Shot 2021-09-30 at 11.45.32 PM" style="zoom:50%;" />

     * State: position err, linear velocity, roll and pitch, roll and pitch velocity
     * output: linear and angular accel
     * use reparameterization trick to sample actions

   * sim2real converter

     <img src="./Screen Shot 2021-09-30 at 11.09.52 PM.png" alt="Screen Shot 2021-09-30 at 11.09.52 PM" style="zoom:50%;" />

     * the policy network + action sampler + action scaler altogether output the desired linear and angular accel
     * sim2real converter acts as a physical model (assumes uniform-accel motion)

3. Training

   * (TODO)

4. Future work

   * extend our work to scenarios with larger tilted angles using a more dexterous quadrotor
   * feed the gap???s tilt angle to the network input, which can facilitate our proposed method to address varying tilting angles without the need to re-train the model

5. my comments

   * don't quite understand how the sim2real converter solves the **error tolerance** problem or the sim-real **model mismatch** problem mentioned in **Control of a Quadrotor with Reinforcement Learning (ETHZ)**
   * has the same level of E2E implementation with the imitation approach, relies on a cascaded system to complete the whole task
   * the major contribution is obtaining the "plan-control" network by learning and without excessive priors
   * the "Training" part is a TODO (study RL before reading this part)
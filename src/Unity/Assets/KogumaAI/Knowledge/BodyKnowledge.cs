using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using SprCs;

public class BodyKnowledge : Knowledge{
    public CRBodyBehaviour crBodyBehaviour;
    public void init(CRBodyBehaviour crBodyBehaviour)
    {
        this.crBodyBehaviour = crBodyBehaviour;
    }
    public List<PHSolidIf> getSolids() {
        List<PHSolidIf> solids = new List<PHSolidIf>();
        for (int i = 0; i < crBodyBehaviour.GetCRBody().NBones(); i++ ) {
            //Debug.Log("body = " + crBodyBehaviour.crBody.GetBone(i).GetName());
            solids.Add(crBodyBehaviour.GetCRBody().GetBone(i).GetPHSolid());
        }
        return solids;
    }
	public Dictionary<string, System.Object> config = new Dictionary<string, System.Object>(){
		//Creature and Body Settings
		{"creature_name", "Koguma"},
		{"no_contacts", new List<Dictionary<string, List<string>>>(){
				new Dictionary<string, List<string>>(){
					{"so_Waist", new List<string>(){ "so_Chest" }},
					{"so_LeftUpperLeg", new List<string>(){ "so_Abdomen", "so_LeftLowerLeg",  "so_LeftFoot"}},
					{"so_RightUpperLeg", new List<string>(){ "so_Abdomen", "so_LeftUpperLeg", "so_RightLowerLeg", "so_RightFoot"}},
					{"so_LeftUpperArm", new List<string>(){ "so_Head" }},
					{"so_RightUpperArm", new List<string>(){ "so_Head" }},
					{"so_LeftLowerArm", new List<string>(){ "so_Head" }},
					{"so_RightLowerArm", new List<string>(){ "so_Head" }},

					//{"so_RightUpperLeg", new List<string>(){ "so_LeftUpperLeg" }},
					{"so_RightFoot", new List<string>(){ "so_LeftFoot" }},

					//{"so_RightUpperLeg", new List<string>(){ "so_RightLowerLeg", "so_RightFoot" }},
					//{"so_LeftUpperLeg", new List<string>(){ "so_LeftLowerLeg",  "so_LeftFoot" }},
				}
			}},
		
		//Perception Settings
		{"floor_name","Floor"},
		{"attention_visualizer_name", "Attention"},

		//Look Settings
		{"enable_look", true},
		{"head_name", "so_Head"},
		{"look_max_margin", (Math.PI / 180) * 25},
		{"look_margin_att_lower_th", 0.0},
		{"look_margin_att_higher_th", 0.7},
		{"head_look_ctl", "HeadLook"},
		{"look_avg_speed", 5.0},
		{"look_wait_vel", 5.0},
		{"look_restart_vel", 2.5},

		//Eye Settings
		{"enable_eyemovement", true},
		{"eyes_name", new Dictionary<string, string>(){ {"L","so_LeftEye"}, {"R","so_RightEye"} }},
		//黒目が中心に来る時のx, yの値
		{"lefteye_center_xy", new Vec2d(-0.05f,  0.08f)},
		{"righteye_center_xy", new Vec2d(-0.05f, -0.08f)},
		{"eye_limit_angle", (Math.PI / 180) * 25},

		//Touch Settings
		{"enable_touch", true},
		{"hands_name", new Dictionary<string, string>(){ {"L","so_LeftHand"}, {"R","so_RightHand"} }},
		{"arms_joint_name", new Dictionary<string, string>(){ {"L","jo_LeftUpperArm"}, {"R","jo_RightUpperArm"} }},
		{"hands_reach_ctl", new Dictionary<string, string>(){ {"LR","HandsReach"}, {"L","LeftReach"}, {"R","RightReach"} }},	
		{"hand_reach_avg_speed", 30.0},
		{"hand_reach_wait_vel", 20.0},
		{"hand_reach_restart_vel", 10.0},

		//Grab Settings
		//* Enable Touch control when you want to use Grab control. *
		{"enable_grab", true},

		//Eat Settings
		//* Enable Look control when you want to use Eat control. *
		{"enable_eat", true},
		{"head_ik_tips", new Dictionary<string, string>(){ {"Look","ike_Head_Tip"}, {"Eat","ike_Head_mouthTip"} }},
		{"hands_ik_tips", new Dictionary<string, string>(){ {"L","ike_LeftHand_Tip"}, {"R","ike_RightHand_Tip"} }},
		{"max_eat_candidates", 5},
		{"eat_start_distance", 4.0},
		{"eat_resign_distance", 5.0},
		{"eat_mouth_start_open", 4.0},
		{"eat_mouth_compl_open", 1.5},
		{"eat_complete_time", 0.8},
		{"wait_after_eat", 2.0}
	};


}

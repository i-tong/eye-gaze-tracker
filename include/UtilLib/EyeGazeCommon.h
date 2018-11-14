#ifndef EYEGAZECOMMON_H
#define EYEGAZECOMMON_H

namespace rclgaze {
	enum eyeSide {
		RIGHT_EYE,
		LEFT_EYE,
		BOTH_EYES
	};
	enum calibType {
		CALIB_2D,
		CALIB_3D
	};
	/*
	struct timeData {
		float time;
		long long tick;
	};
	struct pogValid {
		bool pog_valid_left;
		bool pog_valid_right;
		bool pog_valid_best;
	};*/

	struct OpenAPIDataRecord { // See Section 5 of Gazepoint Open Gaze API
		// Counters
		//int CNT; /*!< The counter data variable is incremented by 1 for each data record sent by the server. Useful to determine if any data packets are missed by the client. */
                double TIME;
		long long TIME_TICK; 

        long long FRAME_CNT;
		float FPOGX;
		float FPOGY;
		float FPOGS;
		float FPOGD;
		int FPOGID;
		bool FPOGV;

		// Left Eye POG
		float LPOGX;
		float LPOGY;
		bool LPOGV;

		// Right Eye POG
		float RPOGX;
		float RPOGY;
		bool RPOGV;

		// Best POG
		float BPOGX;
		float BPOGY;
		bool BPOGV;

		// Left Eye Pupil
		float LPCX;
		float LPCY;
		float LPD;
		float LPS;
		bool LPV;

		// Right Eye Pupil
		float RPCX;
		float RPCY;
		float RPD;
		float RPS;
		bool RPV;

		// Left Eye 3D Data - N/A
		// Right Eye 3D Data - N/A

		// Cursor position
		float CX;
		float CY;
		float CS;

		// User data
		float user_data;
	};

	struct log_data {
                double timestamp;
		long timetick;
        long framecount;
		float pupil_right_x;
		float pupil_right_y;

		float pog_right_x;
		float pog_right_y;
		
		float glint0_right_x;
		float glint0_right_y;
		float glint1_right_x;
		float glint1_right_y;
		float glint2_right_x;
		float glint2_right_y;

		float pg0_right_x;
		float pg0_right_y;
		float pg1_right_x;
		float pg1_right_y;
		float pg2_right_x;
		float pg2_right_y;

		float pupil_right_width;
		float pupil_right_height;
		float pupil_right_angle;

		float eye_right_found;
		float pog_right_valid;

		float pupil_left_x;
		float pupil_left_y;

		float pog_left_x;
		float pog_left_y;

		float glint0_left_x;
		float glint0_left_y;
		float glint1_left_x;
		float glint1_left_y;
		float glint2_left_x;
		float glint2_left_y;

		float pg0_left_x;
		float pg0_left_y;
		float pg1_left_x;
		float pg1_left_y;
		float pg2_left_x;
		float pg2_left_y;

		float pupil_left_width;
		float pupil_left_height;
		float pupil_left_angle;

		float eye_left_found;
		float pog_left_valid;

		float calibration_x;
		float calibration_y;
		bool  calibration_valid;
	};
}

#endif


struct MDSYSTEM {
	double dt; /* timestep         */
	double m;  /* mass             */
	double k;  /* force constant   */
	double a;  /* acceleration     */
	double f;  /* force            */
	double v;  /* velocity         */
	double q;  /* coordinate       */
	double T;  /* temperature      */
	double E;  /* total energy     */
	double Ek; /* kinetic energy   */
	double Ep; /* potential energy */
};

#define k_B 1.0

double gasdev(void);
void vv2(struct MDSYSTEM *mdsystem);
void cs4(struct MDSYSTEM *mdsystem);
void update_a(struct MDSYSTEM *mdsystem);
void update_v(struct MDSYSTEM *mdsystem);
void update_q(struct MDSYSTEM *mdsystem);
void update_f(struct MDSYSTEM *mdsystem);
void update_E(struct MDSYSTEM *mdsystem);
void update_T(struct MDSYSTEM *mdsystem);
void update_Ek(struct MDSYSTEM *mdsystem);
void update_Ep(struct MDSYSTEM *mdsystem);
void thermostat_lan(struct MDSYSTEM *mdsystem, double tau, double temp_target);
void print_task_info(int pos);
void print_help(int argc, char *argv[]);
void print_data(struct MDSYSTEM mdsystem, int step);

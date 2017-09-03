/********************************************//**
 * \file	sc.cpp
 * \brief	Computer Simulation.
 * \author	Wojciech Tyczyñski (tyczynskiwojciech@gmail.com)
 * \date	2014-04-13
 ***********************************************/
#include "sc.h"
#include "students_t_distribution.h"
//================================================
double sim_clock = 0.0;
EventList agenda;
ofstream msg2;
//================================================
int main()
{
	// ------ CREATING NECESSARY OBJECTS FOR MODEL ------
	int routing[4][5] = {		// routing tables
		{0, 1, 1, 2, 2}, {1, 0, 3, 2, 2}, {1, 1, 0, 2, 2}, {2, 3, 4, 0, 1}
	};
	Router routers[NR_OF_ROUTERS] =
	{
		Router(0, 3, routing[0]), Router(1, 4, routing[1]), 
		Router(2, 3, routing[2]), Router(3, 5, routing[3])
	};
	Computer computers[NR_OF_PCS] = {0, 1, 2, 3, 4};
	Link links[NR_OF_LINKS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	// ------ PLUGGING LINKS & DEVICES ------
	bool plugged = true;
	int i = 0;
	while(i < NR_OF_PCS && plugged)
	{
		plugged = connect(links[i], computers[i]);
		++i;
	}

	// to see what's going on down below, check routing table and picture of network in pdfs.
	if(!plugged || !connect(links[0], routers[0], 1, 0) || !connect(links[1], routers[1], 1, 0) ||
		!connect(links[2], routers[2], 1, 0) || !connect(links[3], routers[3], 1, 0) ||
		!connect(links[4], routers[3], 1, 1) || !connect(links[5], routers[0], 0, 1) ||
		!connect(links[5], routers[1], 1, 1) || !connect(links[6], routers[0], 0, 2) ||
		!connect(links[6], routers[3], 1, 2) || !connect(links[7], routers[1], 0, 2) ||
		!connect(links[7], routers[3], 1, 3) || !connect(links[8], routers[1], 0, 3) ||
		!connect(links[8], routers[2], 1, 1) || !connect(links[9], routers[2], 0, 2) ||
		!connect(links[9], routers[3], 1, 4))
	{
		cout << "Connection of devices FAILED!\n";
		return 100;
	}

	// -------- THINGS NECESSARY TO SIMULATION ------------
	double lambda_2 = 1/3.0;			// P2
	Event *current = 0;
	
	cout << "Type value of P1: ";
	int p1;
	cin >> p1;
	while(p1 < 1)
	{
		cout << "Wrong value! Type P1 again: ";
		cin >> p1;
	}

	int M;
	cout << "Type number of simulations: ";
	cin >> M;
	while(M < 1 || M > 10)
	{
		cout << "Wrong value! Type number again: ";
		cin >> M;
	}

	double max_time;
	cout << "Type time of simulation (greater than 10.0): ";
	cin >> max_time;
	while(max_time < 10.0)
	{
		cout << "Wrong value! Type time again: ";
		cin >> max_time;
	}

	double est_mu_link[2][NR_OF_LINKS] = {0};	// Mean busyness of links.
		
	double est_mu_lm[2] = {0};					// For lost messages.
	double est_var_lm[2] = {0};					// Lost messages variance.
	double est_sigma_lm[2] = {0};
	double est_lm[2] = {0};						// Temporary variable.
	double delta_lm[2] = {0};

	double est_mu_md = 0;			// for max delay
	double est_var_md = 0;
	double est_sigma_md = 0;
	double est_delta_md = 0;

	double est_mu_ad[2] = {0};			// for average delay
	double est_var_ad[2] = {0};
	double est_sigma_ad[2] = {0};
	double est_delta_ad[2] = {0};

	//static int seeds[5]={4398577, 3699263, 2899261, 1260113,6332591};

	int seeds[2][NR_OF_PCS] = {20001, 40003, 60007, 70009, 90011, 100013, 120017, 140021, 160027, 190031};
	ofstream stats, stats_l;	// For stats in every iteration.
	stats.open("./output/stats" + to_string(p1) + ".csv");
	stats_l.open("./output/l_stats" + to_string(p1) + ".csv");
	stats << "M;Lost messages 0;Lost messages 1;Max delay;Average delay 0;Average delay 1\n";
	stats_l << "M;link 00;link 01;link 10;link 11;link 20;link 21;link 30;link 31;link 40;link 41;"
		<< "link 50;link 51;link 60;link 61;link 70;link 71;link 80;link 81;link 90;link 91\n";

	for(int m = 0; m < M; ++m)
	{
		//------------- INITIALIZE MODEL -------------

		sim_clock = 0.0;
		msg2.open("./output/msg2_" + to_string(p1) + "_" + to_string(m) + ".log");

		for(int i = 0; i < NR_OF_PCS; ++i)
		{
			agenda.schedule(new GenerateMessage(&computers[i], 1.0 / p1, 0, seeds[0][i]));
			agenda.schedule(new GenerateMessage(&computers[i], lambda_2, 1, seeds[1][i]));
		}

		// ---------------------------------------------------
		// ---------- SIMULATION LOOP (ABC METHOD) -----------
		// ---------------------------------------------------

		// ------------ TRANSIENT PHASE ----------------------
	
		while(sim_clock < 10.0)
		{
			// Get and execute time event first.
			current = agenda.remove_first();
			sim_clock = current->event_time;
			current->execute();
			delete current;

			// Check if you can start sending messages through links.
			for(int i = 0; i < NR_OF_PCS; ++i)
			{
				// Link must be available and buffer can not be empty.
				if(computers[i].check_link() && computers[i].get_buffer()->get_actual_size() != 0)
				{
					computers[i].send_msg();
					agenda.schedule(new EndTransmission(computers[i].link, 0));
				}
			}

			for(int i = 0; i < NR_OF_ROUTERS; ++i)
			{
				if(routers[i].get_buffer()->get_actual_size() == 0) continue;	// Check if message is in router.
				for(int j = 0; j < routers[i].get_nr_of_devices(); ++j)
				{
					if(routers[i].get_buffer()->check_msg_list(j)) continue;	// Check if message is in specified queue.
					int l = (routers[i].links[j]->devices[0] == &routers[i]) ? 0 : 1;
					if(routers[i].check_link(j, l))
					{
						routers[i].send_msg(j);
						agenda.schedule(new EndTransmission(routers[i].links[j], l));
					}
				}
			}
		}

		// Clean stats before steady state
		Message::reset_statistics();
		for(int i = 0; i < NR_OF_LINKS; ++i)
		{
			links[i].reset_statistics();
		}
			
		double steady_state = sim_clock;

		// ----------- STEADY STATE --------------------

		while(sim_clock < max_time)
		{
			current = agenda.remove_first();
			sim_clock = current->event_time;
			current->execute();
			delete current;

			// Check if you can start sending messages through links.
			for(int i = 0; i < NR_OF_PCS; ++i)
			{
				// Link must be available and buffer can not be empty.
				if(computers[i].check_link() && computers[i].get_buffer()->get_actual_size() != 0)
				{
					computers[i].send_msg();
					agenda.schedule(new EndTransmission(computers[i].link, 0));
				}
			}

			for(int i = 0; i < NR_OF_ROUTERS; ++i)
			{
				if(routers[i].get_buffer()->get_actual_size() == 0) continue;	// Check if message is in router.
				for(int j = 0; j < routers[i].get_nr_of_devices(); ++j)
				{
					if(routers[i].get_buffer()->check_msg_list(j)) continue;	// Check if message is in specified queue.
					int l = (routers[i].links[j]->devices[0] == &routers[i]) ? 0 : 1;
					if(routers[i].check_link(j, l))
					{
						routers[i].send_msg(j);
						agenda.schedule(new EndTransmission(routers[i].links[j], l));
					}
				}
			}
		}
		

		// --------- GET STATISTICS ----------
			
		int msgs_delivered[2], msgs_lost[2];
		double busy[NR_OF_LINKS][2];
		Message::get_msgs_delivered(msgs_delivered);
		Message::get_msgs_lost(msgs_lost);
		est_lm[0] = (double)msgs_lost[0] / (msgs_lost[0] + msgs_delivered[0])/* * 100.0*/;
		est_lm[1] = (double)msgs_lost[1] / (msgs_lost[1] + msgs_delivered[1])/* * 100.0*/;

		cout << "P1 = " << p1 << ", simulation nr: " << m << endl;
		cout << "\t\t\tP1\tP2\n";
		cout << "Delivered:\t\t" << msgs_delivered[0] << "\t" << msgs_delivered[1] << endl;
		cout << "Lost:\t\t\t" << msgs_lost[0] << "\t" << msgs_lost[1] << endl;
		cout << "Percentage of lost:\t" << est_lm[0] * 100.0 << "%\t"
			<< est_lm[1] * 100.0 << "%\n";
		cout << "==================================================" << endl;


		est_mu_lm[0] += est_lm[0];
		est_mu_lm[1] += est_lm[1];
		est_var_lm[0] += est_lm[0] * est_lm[0];
		est_var_lm[1] += est_lm[1] * est_lm[1];
			
		double md = Message::get_max_delay();
		est_mu_md += md;
		est_var_md += md * md;

		double ad[2] = { Message::get_average_delay(0), Message::get_average_delay(1) };
		est_mu_ad[0] += ad[0];
		est_mu_ad[1] += ad[1];
		est_var_ad[0] += ad[0] * ad[0];
		est_var_ad[1] += ad[1] * ad[1];

		cout << "Max delay: " << md << endl
			<< "Average delay (0): " << ad[0] << endl
			<< "Average delay (1): " << ad[1] << endl;
		cout << "==================================================" << endl;

		stats << to_string(m) << ';' << to_string(est_lm[0] * 100.0) << ';' << to_string(est_lm[1] * 100.0) << ';'
			<< to_string(md) << ';' << to_string(ad[0]) << ';' << to_string(ad[1]) << endl;
		stats_l << to_string(m) << ';';

		cout << "\t\t\t0\t1\n";
		for(int i = 0; i < NR_OF_LINKS; ++i)
		{
			links[i].get_busy(busy[i]);
			double busyy[2] = {busy[i][0]/(sim_clock-steady_state) * 100.0, busy[i][1]/(sim_clock-steady_state) * 100.0};
			cout << "Link " << i << " was busy: " << busyy[0] << "%\t"
				<< busyy[1] << "%\n";
			est_mu_link[0][i] += busyy[0];
			est_mu_link[1][i] += busyy[1];

			stats_l << to_string(busyy[0]) << ';' << to_string(busyy[1]) << ';';
		}
		stats_l << endl;
		cout << endl;
		
		// --------- REMOVING MESSAGES AND STATISTICS ------------

		for(int i = 0; i < NR_OF_LINKS; ++i)
		{
			links[i].reset_statistics();
			links[i].delete_msgs();
		}
		for(int i = 0; i < NR_OF_PCS; ++i)
			computers[i].get_buffer()->reset_buffer();
		for(int i = 0; i < NR_OF_ROUTERS; ++i)
			routers[i].get_buffer()->reset_buffer();
		Message::reset_statistics();
		agenda.make_empty();

		msg2.close();
	}

	stats.close();
	stats_l.close();

	// --------- PROCESS STATISTICS FOR SPECIFIC LAMBDA ------------
	// ------------- AFTER M SIMULATIONS AND PRINT -----------------


	ofstream model, bs_links;
	model.open("./output/model_stats" + to_string(p1) + ".csv");
	model << "P1;Lost messages 0;Delta of lost 0;Lost messages 1;Delta of lost 1;"
		<< "Max delay;Delta of max delay;Av delay 0;Delta av 0;Av delay 1;Delta av 1\n";
	bs_links.open("./output/link_stats" + to_string(p1) + ".csv");
	bs_links << "P1;";
	for(int i = 0; i < NR_OF_LINKS; ++i)
		bs_links << i << "0;" << i << "1;"; 
	bs_links << endl;

	model << p1 << ';';
	bs_links << p1 << ';';
	cout << "For SIMULATION with P1 = " << p1 << " in the mean:" << endl;
	cout << "\t\t\t0\t1\n";
	for(int i = 0; i < NR_OF_LINKS; ++i)
	{
		cout << "Link " << i << " was busy: " << est_mu_link[0][i] / M << "%\t" << est_mu_link[1][i] / M << "%\n";
		bs_links << est_mu_link[0][i] /M << ';' << est_mu_link[1][i] / M << ';';
	}
	cout << endl;
	bs_links << endl;
		
	enum
	{
		alpha0_1 = 0,
		alpha0_05 = 1,
		alpha0_025 = 2,
		alpha0_01 = 3,
		alpha0_005 = 4
	};

	cout << "Mean of lost messages:\n";

	for(int i = 0; i < 2; ++i)
	{
		est_mu_lm[i] /= M;
		est_var_lm[i] = (1.0 / (M - 1.0)) * (est_var_lm[i] - M * est_mu_lm[i] * est_mu_lm[i]);
		est_sigma_lm[i] = sqrt(est_var_lm[i]);
		delta_lm[i] = st_distribution(M - 1, alpha0_005) * est_sigma_lm[i] / sqrt(M);

		est_mu_ad[i] /= M;
		est_var_ad[i] = (1.0 / (M - 1.0)) * (est_var_ad[i] - M * est_mu_ad[i] * est_mu_ad[i]);
		est_sigma_ad[i] = sqrt(est_var_ad[i]);
		est_delta_ad[i] = st_distribution(M - 1, alpha0_005) * est_sigma_ad[i] / sqrt(M);

		est_mu_lm[i] *= 100.0;
		delta_lm[i] *= 100.0;

		cout << "Priority " << i << ": " << est_mu_lm[i] << " +/- " << delta_lm[i] << "%" << endl;
		model << est_mu_lm[i] << ';' << delta_lm[i] << ';';
	}

	est_mu_md /= M;
	est_var_md = (1.0 / (M - 1.0)) * (est_var_md - M * est_mu_md * est_mu_md);
	est_sigma_md = sqrt(est_var_md);
	est_delta_md = st_distribution(M - 1, alpha0_005) * est_sigma_md / sqrt(M);
	cout << "\nMean max delay = " << est_mu_md << " +/- " << est_delta_md << endl;
	cout << "Mean of delays (0) = " << est_mu_ad[0] << " +/- " << est_delta_ad[0] << endl;
	cout << "Mean of delays (1) = " << est_mu_ad[1] << " +/- " << est_delta_ad[1] << endl;
	cout << endl;
	model << est_mu_md << ';' << est_delta_md << ';' << est_mu_ad[0] << ';' << est_delta_ad[0] << ';' 
		<< est_mu_ad[1] << ';' << est_delta_ad[1] << endl;

	model.close();
	bs_links.close();
	cout << "=================================================="
		<< "\nEnd of simulation.\n";

	// For histograms.
	ofstream randf;
	randf.open("./output/uni.log");
	for(int i = 0; i < 1000; ++i)
		randf << Uniform() << endl;

	randf.close();
	randf.open("./output/exp.log");
	for(int i = 0; i < 1000; ++i)
		randf << Exponential(1/3.) << endl;
	randf.close();

	system("pause");

	return 0;
}
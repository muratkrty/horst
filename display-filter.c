/******************* FILTER *******************/

#include <stdlib.h>

#include "display.h"
#include "main.h"
#include "util.h"

#define CHECKED(_x) (conf.filter_pkt & (_x)) ? '*' : ' '
#define CHECK_ETHER(_mac) MAC_NOT_EMPTY(_mac) ? '*' : ' '
#define CHECK_FILTER_EN(_i) conf.filtermac_enabled[_i] ? '*' : ' '
#define MAC_COL 30

extern WINDOW *filter_win;

void
update_filter_win(void)
{
	int l, i;

	box(filter_win, 0 , 0);
	print_centered(filter_win, 0, 57, " Edit Packet Filter ");

	mvwprintw(filter_win, 2, 2, "Show these Packet Types");

	l = 4;
	wattron(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, 2, "m: [%c] MANAGEMENT FRAMES", CHECKED(PKT_TYPE_MGMT));
	wattroff(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, 2, "b: [%c] Beacons", CHECKED(PKT_TYPE_BEACON));
	mvwprintw(filter_win, l++, 2, "p: [%c] Probe Req/Resp", CHECKED(PKT_TYPE_PROBE));
	mvwprintw(filter_win, l++, 2, "a: [%c] Association", CHECKED(PKT_TYPE_ASSOC));
	mvwprintw(filter_win, l++, 2, "u: [%c] Authentication", CHECKED(PKT_TYPE_AUTH));
	l++;
	wattron(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, 2, "c: [%c] CONTROL FRAMES", CHECKED(PKT_TYPE_CTRL));
	wattroff(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, 2, "r: [%c] CTS/RTS", CHECKED(PKT_TYPE_CTS | PKT_TYPE_RTS));
	mvwprintw(filter_win, l++, 2, "k: [%c] ACK", CHECKED(PKT_TYPE_ACK));
	l++;
	wattron(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, 2, "d: [%c] DATA FRAMES", CHECKED(PKT_TYPE_DATA));
	wattroff(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, 2, "n: [%c] Null Data", CHECKED(PKT_TYPE_NULL));
	mvwprintw(filter_win, l++, 2, "R: [%c] ARP", CHECKED(PKT_TYPE_ARP));
	mvwprintw(filter_win, l++, 2, "P: [%c] ICMP/PING", CHECKED(PKT_TYPE_ICMP));
	mvwprintw(filter_win, l++, 2, "I: [%c] IP", CHECKED(PKT_TYPE_IP));
	mvwprintw(filter_win, l++, 2, "U: [%c] UDP", CHECKED(PKT_TYPE_UDP));
	mvwprintw(filter_win, l++, 2, "T: [%c] TCP", CHECKED(PKT_TYPE_TCP));
	mvwprintw(filter_win, l++, 2, "O: [%c] OLSR", CHECKED(PKT_TYPE_OLSR));
	mvwprintw(filter_win, l++, 2, "B: [%c] BATMAN", CHECKED(PKT_TYPE_BATMAN));

	l = 4;
	wattron(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, MAC_COL, "BSSID");
	wattroff(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, MAC_COL, "s: [%c] %s",
		CHECK_ETHER(conf.filterbssid), ether_sprintf(conf.filterbssid));

	l++;
	mvwprintw(filter_win, l++, MAC_COL, "Show only these");
	wattron(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, MAC_COL, "Source MAC ADDRESSES");
	wattroff(filter_win, A_BOLD);

	for (i = 0; i < MAX_FILTERMAC; i++) {
		mvwprintw(filter_win, l++, MAC_COL, "%d: [%c] %s", i+1,
			CHECK_FILTER_EN(i), ether_sprintf(conf.filtermac[i]));
	}

	l++;
	wattron(filter_win, A_BOLD);
	mvwprintw(filter_win, l++, MAC_COL, "o: [%c] All Filters Off", conf.filter_off ? '*' : ' ' );
	wattroff(filter_win, A_BOLD);

	print_centered(filter_win, 24, 57, "[ Press key or ENTER ]");

	wrefresh(filter_win);
}

void
filter_input(int c)
{
	char buf[18];
	int i;

	switch (c) {
	case 'm':
		TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_MGMT);
		if (conf.filter_pkt & PKT_TYPE_MGMT)
			conf.filter_pkt |= PKT_TYPE_ALL_MGMT;
		else
			conf.filter_pkt &= ~PKT_TYPE_ALL_MGMT;
		break;
	case 'b': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_BEACON); break;
	case 'p': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_PROBE); break;
	case 'a': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_ASSOC); break;
	case 'u': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_AUTH); break;
	case 'c':
		TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_CTRL);
		if (conf.filter_pkt & PKT_TYPE_CTRL)
			conf.filter_pkt |= PKT_TYPE_ALL_CTRL;
		else
			conf.filter_pkt &= ~PKT_TYPE_ALL_CTRL;
		break;
	case 'r': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_CTS | PKT_TYPE_RTS); break;
	case 'k': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_ACK); break;
	case 'd':
		TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_DATA);
		if (conf.filter_pkt & PKT_TYPE_DATA)
			conf.filter_pkt |= PKT_TYPE_ALL_DATA;
		else
			conf.filter_pkt &= ~PKT_TYPE_ALL_DATA;
		break;
	case 'n': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_NULL); break;
	case 'R': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_ARP); break;
	case 'P': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_ICMP); break;
	case 'I': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_IP); break;
	case 'U': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_UDP); break;
	case 'T': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_TCP); break;
	case 'O': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_OLSR|PKT_TYPE_OLSR_LQ|PKT_TYPE_OLSR_GW); break;
	case 'B': TOGGLE_BIT(conf.filter_pkt, PKT_TYPE_BATMAN); break;

	case 'q': case 'Q':
		finish_all(0);

	case 'f': case 'F': case '\r': case KEY_ENTER:
		delwin(filter_win);
		filter_win = NULL;
		update_display(NULL, NULL);
		return;

	case 's':
		echo();
		print_centered(filter_win, 24, 57, "[ Enter new BSSID and ENTER ]");
		mvwgetnstr(filter_win, 5, MAC_COL + 7, buf, 17);
		noecho();
		convert_string_to_mac(buf, conf.filterbssid);
		break;

	case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		i = c - '1';
		if (MAC_NOT_EMPTY(conf.filtermac[i]) && conf.filtermac_enabled[i]) {
			conf.filtermac_enabled[i] = 0;
		}
		else {
			echo();
			print_centered(filter_win, 24, 57, "[ Enter new MAC %d and ENTER ]", i+1);
			mvwgetnstr(filter_win, 9 + i, MAC_COL + 7, buf, 17);
			noecho();
			/* just enable old MAC if user pressed return only */
			if (*buf == '\0' && MAC_NOT_EMPTY(conf.filtermac[i]))
				conf.filtermac_enabled[i] = 1;
			else {
				convert_string_to_mac(buf, conf.filtermac[i]);
				if (MAC_NOT_EMPTY(conf.filtermac[i]))
					conf.filtermac_enabled[i] = true;
			}
		}
		break;

	case 'o':
		conf.filter_off = conf.filter_off ? 0 : 1;
		break;

	default:
		return;
	}

	/* convenience: */
	/* if one of the individual mgmt frames is deselected we dont want to see all mgmt frames */
	if ((conf.filter_pkt & PKT_TYPE_ALL_MGMT) != PKT_TYPE_ALL_MGMT)
		conf.filter_pkt = conf.filter_pkt & ~PKT_TYPE_MGMT;
	/* same for ctl */
	if ((conf.filter_pkt & PKT_TYPE_ALL_CTRL) != PKT_TYPE_ALL_CTRL)
		conf.filter_pkt = conf.filter_pkt & ~PKT_TYPE_CTRL;
	/* same for data */
	if ((conf.filter_pkt & PKT_TYPE_ALL_DATA) != PKT_TYPE_ALL_DATA)
		conf.filter_pkt = conf.filter_pkt & ~PKT_TYPE_DATA;

	/* recalculate filter flag */
	conf.do_macfilter = 0;
	for (i = 0; i < MAX_FILTERMAC; i++) {
		if (conf.filtermac_enabled[i])
			conf.do_macfilter = 1;
	}

	update_filter_win();
}
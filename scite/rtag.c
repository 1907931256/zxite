int v = 0;
int q = 1;

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ESC	"\x1B["
#define TAB     ESC "40G"
#define OFF	ESC "0m"
#define BG_OFF	ESC "0;49m"
#define RED	ESC "0;31m"
#define RED2	ESC "1;31m"

#define MMM fprintf(stderr, "__%s__%d__\n", __FUNCTION__, __LINE__);
#define RRR fprintf(stderr, "__%s__%d__ ->%p\n", __FUNCTION__, __LINE__, __builtin_return_address(0) );
#define DDD(x) fprintf(stderr, "__%s__%d__ %d\n", __FUNCTION__, __LINE__, (x)) ;
#define HHH(x) fprintf(stderr, "__%s__%d__ %x\n", __FUNCTION__, __LINE__, (unsigned long) (x)) ;


//#define MAX(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a > _b ? _a : _b; })
//#define MIN(a,b) ({ __typeof__(a) _a=(a); __typeof__(b) _b=(b); _a < _b ? _a : _b; })

#define BUF_LEN 1024

struct buf_cache {
	int fd;
	off_t len; 	// file len
	off_t off;	// last off request by cache_get
	char *p;	// pointer to off
	char *p_beg;	// min valid data in buffer
	char *p_end;	// max valid data in buffer
	int magic0;
	char bl[BUF_LEN];
	char bh[BUF_LEN];
	int magic1;
};

struct buf_cache *cache_new(int fd, off_t len) {
	struct buf_cache *cache = malloc(sizeof(struct buf_cache));
	cache->p = NULL;
	cache->fd = fd;
	cache->len = len;
	cache->magic0 = 0xDEADBEEF;
	cache->magic1 = 0xDEADBEEF;
	return cache;
}


char *cache_fetch_off(struct buf_cache *cache, off_t off)
{
	//printf("cache %zd %zd\n", off, cache->len);

	assert(cache);
	assert(0 <= off);
	assert(off < cache->len);

	if (cache->p) {
		off_t off_beg = cache->off - (cache->p - cache->p_beg);
		off_t off_end = cache->off + (cache->p_end - cache->p);

		//printf("mmm? %zd %zd %zd %zd\n", off_beg, off, off_end, cache->len);

		assert(0 <= off_beg);
		assert(off_end <= cache->len);

		if (off_beg <= off && off < off_end) {
			cache->p += off - cache->off;
			cache->off = off;
			return cache->p;

		} else if (off_end <= off && off < off_end + BUF_LEN) {
			//printf("A--:  %zd  %zd %zd\n", off_end, off, cache->len);
			//printf("ASS:  E:%p  BE:%p  P:%p  O:%zd  OE:%zd\n", cache->p_end, cache->bh + BUF_LEN, cache->p, cache->off, cache->len - cache->off);

			assert(cache->p_end == cache->bh + BUF_LEN);

			cache->p_beg = cache->bl;
			memcpy(cache->bl, cache->bh, BUF_LEN);

			lseek(cache->fd, off_end, SEEK_SET);
			cache->p = cache->bh + off - off_end;
			cache->p_end = cache->bh + read(cache->fd, cache->bh, BUF_LEN);
			cache->off = off;

			//printf("now:  E:%p  BE:%p  P:%zd  O:%zd  OE:%zd\n", cache->p_end, cache->bh + BUF_LEN, cache->p_end - cache->p, cache->off, cache->len - cache->off);
			return cache->p;


		} else if (off_beg - BUF_LEN <= off && off < off_beg) {

			assert(cache->p_beg == cache->bl || cache->p_beg == cache->bh);

			if (cache->p_beg == cache->bl) {
				memcpy(cache->bh, cache->bl, BUF_LEN);
				cache->p_end = cache->bh + BUF_LEN;
			}

			if (off_beg < BUF_LEN) {
				lseek(cache->fd, 0, SEEK_SET);
				cache->p_beg = cache->bl + BUF_LEN - off_beg;
				int len = read(cache->fd, cache->p_beg, off_beg);
				assert(len == off_beg);
			} else {
				cache->p_beg = cache->bl;

				lseek(cache->fd, off_beg - BUF_LEN, SEEK_SET);
				int len = read(cache->fd, cache->p_beg, BUF_LEN);
				assert(len == BUF_LEN);
			}

			cache->p = cache->bh - (off_beg - off);
			cache->off = off;
			return cache->p;
		}
	}

	lseek(cache->fd, off, SEEK_SET);
	cache->p_beg = cache->p = cache->bh;
	cache->p_end = cache->p + read(cache->fd, cache->p, BUF_LEN);

	cache->off = off;
	return cache->p;
}

off_t cache_off(struct buf_cache *cache, char *p)
{
	return cache->off + p - cache->p;
}

char *cache_fetch_p(struct buf_cache *cache, char *p)
{
	assert(cache->p);
	off_t off = cache_off(cache, p);
	//printf("RHRHRH %zd %zd\n", cache->len, off);
	if (off < 0 || cache->len <= off)
		return NULL;
	return cache_fetch_off(cache, off);
}



char *prev_eol(struct buf_cache *cache, off_t off)
{
	char *p = cache_fetch_off(cache, off);

	do {
		p--;
		if (p < cache->p_beg)
			p = cache_fetch_p(cache, p);
	} while (p && *p != '\n');
	return p;
}

char *next_eol(struct buf_cache *cache, off_t off)
{
	char *p = cache_fetch_off(cache, off);
	while (p && *p != '\n') {
		p++;
		if (cache->p_end <= p)
			p = cache_fetch_p(cache, p);
	}
	return p;
}

char **bs(const char *str)
{
	char **res_dyn = NULL;
	int fd = open("tags", O_RDONLY);
	if (fd < 0) {
		perror("OPEN");
		printf("ERR open\n");
		exit(-1);
	}

	off_t beg = 0;
	off_t end = lseek(fd, 0, SEEK_END);
	if (end == (off_t) -1) {
		//perror("LSEEK");
		printf("ERR lseek\n");
		exit(-1);
	}

	off_t mid = (beg + end) / 2;
	struct buf_cache *cache = cache_new(fd, end);

	char *pb = next_eol(cache, mid);
	if (pb == NULL) {
		pb = prev_eol(cache, mid);
		if (pb == NULL)
			goto bs_exit;
	}

	off_t lllast_sol = -1;
	off_t llast_sol = -1;
	off_t last_sol = -1;
	for (;;) {
		//usleep(10000);
		pb = cache_fetch_p(cache, pb + 1);
		mid = cache_off(cache, pb);

		if (v) printf("curr: <%.*s> %zd  (%zd %zd %zd)  ", 32, pb, mid, last_sol, llast_sol, lllast_sol);

		if (mid == llast_sol && lllast_sol == last_sol)
			goto bs_exit;
		lllast_sol = llast_sol;
		llast_sol = last_sol;
		last_sol = mid;

		const char *ps = str;
		for (;;) {
			if (pb >= cache->p_end)
				pb = cache_fetch_p(cache, pb);
			if (pb == NULL)
				goto bs_exit;

			if (*ps == '\0' && *pb == '\t')
				// found
				goto found;
			int diff = *pb++ - *ps++;
			if (diff > 0) {
				//before
				end = mid;
				pb = prev_eol(cache, (beg + end) / 2);
				if (pb == NULL)
					pb = cache_fetch_off(cache, 0) - 1;
				if (v) printf("before %zd - %zd - %zd\n", beg, cache_off(cache, pb + 1), end);
				break;
			}
			if (diff < 0) {
				//after
				beg = mid;
				pb = next_eol(cache, (beg + end) / 2);
				if (pb == NULL)
					goto bs_exit;
				if (v) printf("after %zd - %zd - %zd\n", beg, cache_off(cache, pb + 1), end);
				break;
			}
		}
	}

found:
	if (v) printf("Found %zd!\n\n", mid);

	int str_len = strlen(str);

	//if (v) printf("dbg: %zd - %p - %zd\n", last_sol, pb, mid);
	//if (v) printf("dbg1: %zd - %p - %zd\n", last_sol, pb, mid);

	for (;;) {
		if (mid == 0)
			break;
		//if (v) printf("mid <%zd>\n", mid);
		pb = prev_eol(cache, mid - 1);
		//if (v) printf("mid2 <%zd>\n", cache_off(cache, pb));
		if (pb++ == NULL)
			pb = cache_fetch_off(cache, 0);

		//if (v) printf("try <%.30s>\n", pb);
		if (strncmp(str, pb, str_len) || pb[str_len] != '\t')
			break;
		mid = cache_off(cache, pb);
	}

	if (v) printf("\nmin ok\n");

	#define MAX_CNT 16
	int idx = 0;
	char *res[MAX_CNT + 1];

	char *npb = next_eol(cache, mid);
	assert(npb);

	while (idx < MAX_CNT) {
		pb = cache_fetch_off(cache, mid);
		res[idx++] = strndup(pb, npb - pb);

		if (v) printf("try1 [%d] <%.30s> %p\n", idx, pb, res[idx-1]);
		//if (v) printf("dbg: %zd - %p - %zd\n", last_sol, pb, mid);
		mid = cache_off(cache, npb + 1);
		if (cache->len <= mid)
			break;

		npb = next_eol(cache, mid);
		if (v) printf("B\n");
		if (npb == NULL)
			break;
		if (v) printf("C\n");
		pb = cache_fetch_off(cache, mid);
		if (v) printf("D\n");
		if (strncmp(str, pb, str_len) || pb[str_len] != '\t')
			break;
		if (v) printf("try3 <%.30s>\n", pb);
	}

	if (v) printf("\nmax ok\n");

	res[idx++] = NULL;

	res_dyn = malloc(sizeof(char *) * idx);
	memcpy(res_dyn, res, sizeof(char *) * idx);

bs_exit:
	assert(cache->magic0 == 0xDEADBEEF);
	assert(cache->magic1 == 0xDEADBEEF);
	free(cache);
	close(fd);

	if (v) printf("ret %p\n", res_dyn);

	return res_dyn;
}


void dump_tag (char *tag)
{
	char *s = tag;
	char *filename = NULL;
	long int line = 0;
	char *pattern = NULL;
	char kind = '\0';

	if ((s = strchr(s, '\t')) == NULL)
		goto err;
	filename = ++s;
	if ((s = strchr(s, '\t')) == NULL)
		goto err;
	s[0] = '\0';
	if (s[1] != '/' || s[2] != '^') {
		char *end;
		line = strtol(s + 1, &end, 10);
		if (line < 1)
			goto err;
		s = end;
	} else {
		s += 3;
		pattern = ++s;
		while (s[0] != '$' || s[1] != '/')
			if (!*s++)
				goto err;
		s[0] = '\0';
		s += 2;
	}
	if (*s) {
		if (s[0] != ';' || s[1] != '"')
			goto err;
		s += 2;

		do {
			if (*s++ != '\t')
				goto err;
			char *ext_val = s;
			char *ext_name = s;
			while (*s && *s != '\t')
				if (*s++ == ':')
					ext_val = s;
			if (ext_val == ext_name || !strncmp(ext_name, "kind", ext_val - 1 - ext_name)) {
				if (s - ext_val != 1)
					goto err;
				kind = *ext_val;
				printf("[kind]=%c  ", kind);
			} else if (!strncmp(ext_name, "line", ext_val - 1 - ext_name)) {
				line = strtol(ext_val, &ext_val, 10);
				if (line < 1 || ext_val != s)
					goto err;
			} else if (ext_val < s) {
				printf("[%.*s]=<%.*s>  ", ext_val - 1 - ext_name, ext_name, s - ext_val, ext_val);
			}
		} while (*s);
		printf("[line]=%d  [f]=<%s>  [p]=<%s>\n", line, filename, pattern);
	}

	return;
err:
	printf(RED "Unable to parse %s\n" OFF, tag);
}



void test(void)
{
	int fd = open("tags", O_RDONLY);
	if (fd < -1) {
		printf("ERR open\n");
		exit(-1);
	}

	struct stat fst;
	fstat(fd, &fst);

	char * buf = malloc(fst.st_size + 1);
	int ret = read(fd, buf, fst.st_size);
	assert(ret == fst.st_size);
	buf[fst.st_size] = 0;
	close(fd);

	char *p = buf;

	char sym[128];
	char prev[128] = "";
	int prev_match = 0;

	for (;;) {
		char *tab = strchr(p, '\t');
		if (tab == NULL)
			break;
		char *eol = strchr(tab, '\n');
		assert(eol);
		strncpy(sym, p, tab - p);
		sym[tab - p] = '\0';

		if (strcmp(prev, sym) == 0) {
			prev_match++;
			if (prev_match >= MAX_CNT) {
				if (q) printf("==== Skip: %s (%d)\n", sym, prev_match);
				goto next_sym;
			}
		} else {
			strcpy(prev, sym);
			prev_match = 0;
		}

		//--------------
		if (q) printf("==== Search: <%s>  ", sym);
		char **res = bs(sym);
		//printf("== done == ");
		int i = 0;
		assert(res);
		assert(res[i]);
		while (res[i]) {
			if (v) printf("A<%.*s>\n", (int)(eol - p), res[i]);
			if (v) printf("B<%.*s>\n", (int)(eol - p), p);
			if (strncmp(res[i], p, eol - p) == 0)
				break;
			if (v) printf("C\n");
			i++;
		}
		if (res[i]) {
			if (q) {
				//printf("found: <%s>\n", res[i]);
				dump_tag(res[i]);
			}
		}
		else
			assert(0);
		i = 0;
		while(res[i])
			free(res[i++]);
		free(res);
		goto next_sym;
		//--------------

		sym[tab - p] = 'z';
		sym[tab - p + 1] = 'Z';
		sym[tab - p + 2] = '\0';

		if (q) printf("==== False search: %s... ", sym);
		res = bs(sym);
		//printf("== done == ");
		assert(res == NULL);
		if (q) printf("not found!\n");

		//--------------
		sym[tab - p - 1] = '#';
		sym[tab - p] = '\0';

		if (q) printf("==== False search: %s... ", sym);
		res = bs(sym);
		//printf("== done == ");
		assert(res == NULL);
		if (q) printf("not found!\n");

	next_sym:
		p = eol + 1;
	}


	free(buf);
}


int main (int argc, char *argv[])
{
	if (argc == 1)
		test();
	if (argc == 2) {

		char **res = bs(argv[1]);

		printf("\nres:\n");

		int i = 0;
		while(res && res[i]) {
			printf("%s\n", res[i]);
			dump_tag(res[i]);
			free(res[i++]);
		}
		free(res);
	}

	return 0;
}

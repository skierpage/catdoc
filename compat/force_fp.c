/* Get Borland Turbo C to link in the floating-point library,
 * see http://www.faqs.org/faqs/msdos-programmer-faq/part2/section-5.html
 */
static void forcefloat(float *p)
{
	float f = *p;
	forcefloat(&f);
}

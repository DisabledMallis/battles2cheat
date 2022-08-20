#ifndef SRC_REDSHIFT_DISPLAY
#define SRC_REDSHIFT_DISPLAY

#include "render/renderable.h"
#include "redshift.h"

struct redshift_display : public renderable {
	redshift_display();
	void on_render() override;
};

#endif /* SRC_REDSHIFT_DISPLAY */

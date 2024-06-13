/*
 * packet.h
 *
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>
#include <stddef.h>

#include <common/common.h>
#include <common/object.h>

AWE_BEGIN_DECLS

typedef struct packet{
    OBJECT_DEC;

    int32_t _rangeOffset;
    int32_t _rangeLength;

    uint32_t _dts;

    int32_t _capacity;

    uint8_t *_data;

    bool _ownsData;
}packet;


AWE_DECLARE(packet*) packet_create(int32_t capacity);

AWE_DECLARE(packet*) packet_addref(packet *pkt);
AWE_DECLARE(void) packet_autorelease(packet **pkt);

AWE_DECLARE(void) packet_autorelease_proc(awe_object **o);

static inline uint8_t *packet_base(packet *pkt) {
    return pkt->_data;
}
static inline uint8_t *packet_data(packet *pkt) {
    return pkt->_data + pkt->_rangeOffset;
}
static inline int32_t packet_capacity(packet *pkt) {
    return pkt->_capacity;
}
static inline int32_t packet_size(packet *pkt) {
    return pkt->_rangeLength;
}
static inline int32_t packet_offset(packet *pkt) {
    return pkt->_rangeOffset;
}
static inline uint32_t packet_getdts(packet *pkt){
    return pkt->_dts;
}
static inline void packet_setdts(packet *pkt, uint32_t dts) {
    pkt->_dts = dts;
}

AWE_DECLARE(void) packet_setrange(packet *pkt, int32_t offset, int32_t size);
AWE_DECLARE(void) packet_setrange2(packet *pkt, int32_t offset, int32_t size);
AWE_DECLARE(void) packet_setrange3(packet *pkt, int32_t offset, int32_t size);

AWE_DECLARE(void) packet_unfree_data(packet *pkt, bool b);

AWE_END_DECLS

#endif /* AWE_PACKET_H_ */

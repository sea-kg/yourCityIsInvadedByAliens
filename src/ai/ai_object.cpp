#include "ai_object.h"

// ---------------------------------------------------------------------
// AiObject

AiObject::AiObject() {
    m_bDestroyed = false;
}

void AiObject::setDestroyed(bool bDestroyed) {
    m_bDestroyed = bDestroyed;
}

bool AiObject::isDestroyed() {
    return m_bDestroyed;
}
#include "ProgressView.h"

void ProgressView::setBackgroundTexture(const char * pName, bool texture) {
	if (texture) {
		m_progressBackground = CCSprite::createWithSpriteFrameName(pName);
	} else {
		m_progressBackground = CCSprite::create(pName);
	}
    this->addChild(m_progressBackground);
}

void ProgressView::setForegroundTexture(const char * pName, bool texture) {
	if (texture) {
		m_progressForeground = CCSprite::createWithSpriteFrameName(pName);
	} else {
		m_progressForeground = CCSprite::create(pName);
	}
    m_progressForeground->setAnchorPoint(ccp(0.0f, 0.5f));
	originalWidth = m_progressForeground->getContentSize().width;
    m_progressForeground->setPosition(ccp(-originalWidth * 0.5f, 0));
    this->addChild(m_progressForeground);
}

void ProgressView::setTotalProgress(float total) {
    if (m_progressForeground == NULL) {
		return;
	}
    m_scale = originalWidth / total;
    m_totalProgress = total;
}

void ProgressView::setCurrentProgress(float progress) {
    if (m_progressForeground == NULL) {
		return;
	}
    if (progress < 0.0f) {
		progress = 0.0f;
	}
    if (progress > m_totalProgress) {
		progress = m_totalProgress;
	}
    m_currentProgress = progress;
    float rectWidth = progress * m_scale;
    const CCPoint from = m_progressForeground->getTextureRect().origin;
    const CCRect rect = CCRectMake(from.x, from.y, rectWidth, m_progressForeground->getContentSize().height);
    setForegroundTextureRect(rect);
}

void ProgressView::setForegroundTextureRect(const CCRect &rect) {
    m_progressForeground->setTextureRect(rect);
}

ProgressView::ProgressView()
    : m_progressBackground(NULL)
    , m_progressForeground(NULL)
    , m_totalProgress(0.0f)
    , m_currentProgress(0.0f)
    , m_scale(1.0f)
	, originalWidth(0)
{}

float ProgressView::getCurrentProgress() const {
    return m_currentProgress;
}

float ProgressView::getTotalProgress() const {
    return m_totalProgress;
}


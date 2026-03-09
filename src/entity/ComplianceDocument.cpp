#include "ComplianceDocument.h"

ComplianceDocument::ComplianceDocument()
    : m_type(DocumentType::BUSINESS_LICENCE), m_documentNumber(""),
      m_expirationDate(QDate()), m_insuranceProvider("") {
}

ComplianceDocument::ComplianceDocument(DocumentType type, const QString& documentNumber,
                                       const QDate& expirationDate)
    : m_type(type), m_documentNumber(documentNumber),
      m_expirationDate(expirationDate), m_insuranceProvider("") {
}

ComplianceDocument::~ComplianceDocument() {
}

ComplianceDocument::DocumentType ComplianceDocument::getType() const {
    return m_type;
}

QString ComplianceDocument::getTypeString() const {
    switch (m_type) {
        case DocumentType::BUSINESS_LICENCE:
            return "Business Licence";
        case DocumentType::LIABILITY_INSURANCE:
            return "Liability Insurance";
        case DocumentType::FOOD_HANDLER_CERTIFICATION:
            return "Food Handler Certification";
        default:
            return "Unknown";
    }
}

QString ComplianceDocument::getDocumentNumber() const {
    return m_documentNumber;
}

QDate ComplianceDocument::getExpirationDate() const {
    return m_expirationDate;
}

QString ComplianceDocument::getExpirationDateString() const {
    return m_expirationDate.toString("yyyy-MM-dd");
}

QString ComplianceDocument::getInsuranceProvider() const {
    return m_insuranceProvider;
}

void ComplianceDocument::setType(DocumentType type) {
    m_type = type;
}

void ComplianceDocument::setDocumentNumber(const QString& documentNumber) {
    m_documentNumber = documentNumber;
}

void ComplianceDocument::setExpirationDate(const QDate& expirationDate) {
    m_expirationDate = expirationDate;
}

void ComplianceDocument::setInsuranceProvider(const QString& provider) {
    m_insuranceProvider = provider;
}

bool ComplianceDocument::isExpired() const {
    return m_expirationDate < QDate::currentDate();
}

bool ComplianceDocument::isValidForSeason() const {
    int currentYear = QDate::currentDate().year();
    QDate seasonEnd(currentYear, 9, 30);
    return m_expirationDate > seasonEnd;
}

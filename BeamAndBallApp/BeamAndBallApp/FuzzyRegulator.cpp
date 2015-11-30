#include "pch.h"
#include "FuzzyRegulator.h"


FuzzyRegulator::FuzzyRegulator()
{
}


FuzzyRegulator::~FuzzyRegulator()
{
}

void FuzzyRegulator::Init()
{
	fl::Engine* engine = new fl::Engine("beamAndBallMain");
	engine->configure("AlgebraicProduct", "AlgebraicSum", "Minimum", "Maximum", "Centroid");

	// Input 1
	odchylka = new fl::InputVariable;
	odchylka->setName("odchylka");
	odchylka->setRange(-1.000, 1.000);
	odchylka->addTerm(new fl::Triangle("NB", -fl::inf, -0.75f, -0.5f));
	odchylka->addTerm(new fl::Triangle("NN", -0.75, - 0.5, - 0.25));
	odchylka->addTerm(new fl::Triangle("NS", -0.5, - 0.25, 0));
	odchylka->addTerm(new fl::Triangle("ZE", -0.25, 0, 0.25));
	odchylka->addTerm(new fl::Triangle("PS", 0, 0.25, 0.5));
	odchylka->addTerm(new fl::Triangle("PM", 0.25, 0.5, 0.75));
	odchylka->addTerm(new fl::Triangle("PB", 0.5, 0.75, fl::inf));
	engine->addInputVariable(odchylka);

	// Input 2
	deltaE = new fl::InputVariable;
	deltaE->setName("deltaE");
	deltaE->setRange(-1.000, 1.000);
	odchylka->addTerm(new fl::Triangle("NB", -fl::inf, -0.75f, -0.5f));
	odchylka->addTerm(new fl::Triangle("NN", -0.75, -0.5, -0.25));
	odchylka->addTerm(new fl::Triangle("NS", -0.5, -0.25, 0));
	odchylka->addTerm(new fl::Triangle("ZE", -0.25, 0, 0.25));
	odchylka->addTerm(new fl::Triangle("PS", 0, 0.25, 0.5));
	odchylka->addTerm(new fl::Triangle("PM", 0.25, 0.5, 0.75));
	odchylka->addTerm(new fl::Triangle("PB", 0.5, 0.75, fl::inf));
	engine->addInputVariable(deltaE);

	// Output
	angle = new fl::OutputVariable;
	angle->setName("angle");
	angle->setRange(-1.000, 1.000);
	angle->setDefaultValue(fl::nan);
	odchylka->addTerm(new fl::Triangle("NM3", -fl::inf, -0.75f, -0.5f));
	odchylka->addTerm(new fl::Triangle("NM2", -0.75, -0.5, -0.25));
	odchylka->addTerm(new fl::Triangle("NM1", -0.5, -0.25, 0));
	odchylka->addTerm(new fl::Triangle("NPM", -0.25, 0, 0.25));
	odchylka->addTerm(new fl::Triangle("PM1", 0, 0.25, 0.5));
	odchylka->addTerm(new fl::Triangle("PM2", 0.25, 0.5, 0.75));
	odchylka->addTerm(new fl::Triangle("PM3", 0.5, 0.75, fl::inf));
	engine->addOutputVariable(angle);

	// Rules
	fl::RuleBlock* ruleblock = new fl::RuleBlock;
	ruleblock->setEnabled(true);
	ruleblock->setName("");
	ruleblock->setConjunction(new fl::Minimum());
	ruleblock->setDisjunction(new fl::Maximum());
	ruleblock->setImplication(new fl::Minimum());
	ruleblock->addRule(fl::Rule::parse("if odchylka is NB and deltaE is NB then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NM and deltaE is NB then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NS and deltaE is NB then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is ZE and deltaE is NB then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PS and deltaE is NB then angle is NM2", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PM and deltaE is NB then angle is NS1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PB and deltaE is NB then angle is NPM", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NB and deltaE is NM then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NM and deltaE is NM then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NS and deltaE is NM then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is ZE and deltaE is NM then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PS and deltaE is NM then angle is NS1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PM and deltaE is NM then angle is NPM", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PB and deltaE is NM then angle is PM1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NB and deltaE is NS then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NM and deltaE is NS then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NS and deltaE is NS then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is ZE and deltaE is NS then angle is NS1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PS and deltaE is NS then angle is NPM", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PM and deltaE is NS then angle is PM1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PB and deltaE is NS then angle is PM2", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NB and deltaE is ZE then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NM and deltaE is ZE then angle is NM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NS and deltaE is ZE then angle is NS1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is ZE and deltaE is ZE then angle is NPM", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PS and deltaE is ZE then angle is PM1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PM and deltaE is ZE then angle is PM2", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PB and deltaE is ZE then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NB and deltaE is PS then angle is NM2", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NM and deltaE is PS then angle is NS1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NS and deltaE is PS then angle is NPM", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is ZE and deltaE is PS then angle is PM1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PS and deltaE is PS then angle is PM2", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PM and deltaE is PS then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PB and deltaE is PS then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NB and deltaE is PM then angle is NS1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NM and deltaE is PM then angle is NPM", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NS and deltaE is PM then angle is PM1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is ZE and deltaE is PM then angle is PM2", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PS and deltaE is PM then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PM and deltaE is PM then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PB and deltaE is PM then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NB and deltaE is PB then angle is NPM", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NM and deltaE is PB then angle is PM1", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is NS and deltaE is PB then angle is PM2", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is ZE and deltaE is PB then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PS and deltaE is PB then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PM and deltaE is PB then angle is PM3", engine));
	ruleblock->addRule(fl::Rule::parse("if odchylka is PB and deltaE is PB then angle is PM3", engine));
	engine->addRuleBlock(ruleblock);

	// Stuff
	std::string status;
	if (not engine->isReady(&status))
		throw fl::Exception("Engine not ready. "
		"The following errors were encountered:\n" + status, FL_AT);
}

float FuzzyRegulator::getAngle(float odchylka, float rozdiel)
{
	this->odchylka->setValue(odchylka);
	deltaE->setValue(rozdiel);
	engine->process();
	return angle->getValue();
}
